#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>//EXIT_FAILURE
#include <string.h>
#include <math.h>

#define PI		   3.14159265358979323846  /* pi */
#define TWOPI		6.28318530717958647693

int DBG_ENABLE;
#define DBG_PRINT(format,args...) if (DBG_ENABLE==1) fprintf(stderr,format, ## args);

int IMG_X,IMG_Y,MAX_FALSE_STARS,DB_REDUNDANCY,REQUIRED_STARS;
float PIXSCALE,EXPECTED_FALSE_STARS,DOUBLE_STAR_PX;
float BASE_FLUX,IMAGE_VARIANCE,THRESH_FACTOR,POS_VARIANCE,POS_ERR_SIGMA;
float MAXFOV,MINFOV,MATCH_VALUE,PIXX_TANGENT,PIXY_TANGENT;


int ENV_VARS_SIZE;
char** ENV_VARS; //keep track of these so that valgrind is happy

void load_config(const char *filename) {
	DBG_ENABLE=0;
	ENV_VARS_SIZE=0;
	ENV_VARS=NULL;
	//move calibration.txt to constellation_db
	//move dbstats.txt to beastdb
	/* load config */
	
	FILE *stream = fopen(filename, "r");
	if (stream == NULL) exit(EXIT_FAILURE);

	ssize_t read;
	char *line = NULL;
	size_t len = 0;
	while ((read = getline(&line, &len, stream)) != -1) {
		ENV_VARS=(char**)realloc(ENV_VARS,(ENV_VARS_SIZE+1)*sizeof(ENV_VARS[0]));
		ENV_VARS[ENV_VARS_SIZE]=(char *)malloc(sizeof(char) * len);
		putenv(strcpy(ENV_VARS[ENV_VARS_SIZE++],line));
	}
	free(line);
	fclose(stream);

	IMG_X=atoi(getenv("IMG_X"));
	IMG_Y=atoi(getenv("IMG_Y"));
	PIXSCALE=atof(getenv("PIXSCALE"));
	POS_ERR_SIGMA=atof(getenv("POS_ERR_SIGMA"));
	POS_VARIANCE=atof(getenv("POS_VARIANCE"));/* sigma_r^2 */
	IMAGE_VARIANCE=atof(getenv("IMAGE_VARIANCE"));/* lambda */
	THRESH_FACTOR=atof(getenv("THRESH_FACTOR"));/* lambda */
	EXPECTED_FALSE_STARS=atof(getenv("EXPECTED_FALSE_STARS"));/* pfalse */
	DOUBLE_STAR_PX=atof(getenv("DOUBLE_STAR_PX"));
	MAX_FALSE_STARS=atoi(getenv("MAX_FALSE_STARS"));/* >10 is slow */
	DB_REDUNDANCY=atoi(getenv("DB_REDUNDANCY"));
	REQUIRED_STARS=atoi(getenv("REQUIRED_STARS"));
	BASE_FLUX=atof(getenv("BASE_FLUX"));
	
	MAXFOV=PIXSCALE*sqrt(IMG_X*IMG_X+IMG_Y*IMG_Y);
	MINFOV=PIXSCALE*IMG_Y;
	MATCH_VALUE=4*log(EXPECTED_FALSE_STARS/(IMG_X*IMG_Y))+log(2*PI);/* base */
	PIXX_TANGENT=2*tan((IMG_X*PIXSCALE/3600)*PI/(180*2))/IMG_X;
	PIXY_TANGENT=2*tan((IMG_Y*PIXSCALE/3600)*PI/(180*2))/IMG_Y;
}
#endif