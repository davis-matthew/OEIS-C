/*
 * A162355  Nonnegative integers whose Spanish and English names contain the
 *          same number of letters.
 * https://oeis.org/A162355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for a(n): the n-th nonnegative integer whose English and Spanish
 * names contain the same number of letters (ignoring spaces and hyphens). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* -------- English number component lengths -------- */
static const seq_t eng_units[20] = {
    4, /* 0 zero       */
    3, /* 1 one        */
    3, /* 2 two        */
    5, /* 3 three      */
    4, /* 4 four       */
    4, /* 5 five       */
    3, /* 6 six        */
    5, /* 7 seven      */
    5, /* 8 eight      */
    4, /* 9 nine       */
    3, /*10 ten        */
    6, /*11 eleven     */
    6, /*12 twelve     */
    8, /*13 thirteen   */
    8, /*14 fourteen   */
    7, /*15 fifteen    */
    7, /*16 sixteen    */
    9, /*17 seventeen  */
    8, /*18 eighteen   */
    8  /*19 nineteen   */
};

static const seq_t eng_tens[10] = {
    0,0,          /* 0‑9 */
    6,            /*20 twenty*/
    6,            /*30 thirty*/
    5,            /*40 forty*/
    5,            /*50 fifty*/
    5,            /*60 sixty*/
    7,            /*70 seventy*/
    6,            /*80 eighty*/
    6             /*90 ninety*/
};

static const seq_t eng_scale_len[6] = {0,8,7,7,8,11};

/* -------- Spanish number component lengths -------- */
static const seq_t spa_units[20] = {
    4, /* 0 cero        */
    3, /* 1 uno         */
    3, /* 2 dos         */
    4, /* 3 tres        */
    6, /* 4 cuatro      */
    5, /* 5 cinco       */
    4, /* 6 seis        */
    5, /* 7 siete       */
    4, /* 8 ocho        */
    5, /* 9 nueve       */
    4, /*10 diez        */
    4, /*11 once        */
    4, /*12 doce        */
    5, /*13 trece       */
    7, /*14 catorce     */
    6, /*15 quince      */
    9, /*16 dieciseis   */
    10,/*17 diecisiete  */
    9, /*18 dieciocho   */
    10 /*19 diecinueve  */
};

static const seq_t spa_veinti_prefix = 6; /* "veinti" length */

static const seq_t spa_tens[10] = {
    0,0,          /* 0‑9 */
    6,            /*20 veinte*/
    7,            /*30 treinta*/
    8,            /*40 cuarenta*/
    9,            /*50 cincuenta*/
    7,            /*60 sesenta*/
    7,            /*70 setenta*/
    7,            /*80 ochenta*/
    7             /*90 noventa*/
};

static const seq_t spa_hundreds[10] = {
    0, 0,         /* 0‑99  */
    10,           /*200 doscientos*/
    11,           /*300 trescientos*/
    13,           /*400 cuatrocientos*/
    10,           /*500 quinientos*/
    10,           /*600 seiscientos*/
    11,           /*700 setecientos*/
    11,           /*800 ochocientos*/
    11            /*900 novecientos*/
};

/* -------- Helper: English small number → letter count -------- */
static seq_t english_small ( idx_t n )
{
    if ( n < 20 ) return eng_units[n];
    if ( n < 100 ) {
        if ( n % 10 == 0 ) return eng_tens[n/10];
        return eng_tens[n/10] + eng_units[n % 10];
    }
    /* 100 – 999 */
    seq_t res = eng_units[n / 100] + 7;   /* 'hundred' = 7 */
    if ( n % 100 ) res += english_small( n % 100 );
    return res;
}

/* -------- Helper: Spanish small number → letter count -------- */
static seq_t spanish_small ( idx_t n )
{
    if ( n == 0 ) return 4;               /* 'cero' */
    if ( n == 20 ) return 6;              /* 'veinte' */
    if ( n < 20 ) return spa_units[n];
    if ( n < 30 ) return spa_veinti_prefix + spa_units[n-20];
    if ( n < 100 ) {
        if ( n % 10 == 0 ) return spa_tens[n/10];
        return spa_tens[n/10] + 1 + spa_units[n%10]; /* + 'y' */
    }
    if ( n == 100 ) return 4;             /* 'cien' */
    if ( n < 200 ) {                       /* 101–199: 'ciento' + rest */
        return 6 + spanish_small( n - 100 );
    }
    /* 200 – 999 */
    seq_t res = spa_hundreds[n/100];
    if ( n % 100 ) res += spanish_small( n % 100 );
    return res;
}

/* -------- Helper: Count letters for a uint (English) -------- */
static seq_t english_uint ( idx_t n )
{
    if ( n == 0 ) return eng_units[0];
    uint32_t grp[10];
    int gcnt = 0;
    idx_t rem = n;
    while ( rem ) { grp[gcnt++] = rem % 1000; rem /= 1000; }

    seq_t res = 0;
    for ( int i = gcnt-1; i >= 0; --i ) {
        uint32_t g = grp[i];
        if ( g == 0 ) continue;
        res += english_small( g );
        if ( i > 0 && i < 6 ) res += eng_scale_len[i];
    }
    return res;
}

/* -------- Helper: Count letters for a uint (Spanish) -------- */
static seq_t spanish_uint ( idx_t n )
{
    if ( n == 0 ) return 4; /* 'cero' */
    uint32_t grp[10];
    int gcnt = 0;
    idx_t rem = n;
    while ( rem ) { grp[gcnt++] = rem % 1000; rem /= 1000; }

    seq_t res = 0;
    for ( int i = gcnt-1; i >= 0; --i ) {
        uint32_t g = grp[i];
        if ( g == 0 ) continue;
        seq_t gcnt_val = spanish_small( g );
        if ( i == 1 ) {                  /* thousands */
            if ( g == 1 ) res += 3;         /* 'mil' */
            else           res += gcnt_val + 3;
        } else if ( i == 2 ) {           /* millions */
            if ( g == 1 ) res += 9;         /* 'un millón' */
            else           res += gcnt_val + 8;
        } else if ( i == 3 ) {           /* billions – 'mil millones' */
            if ( g == 1 ) res += 11;        /* 'mil millones' */
            else           res += gcnt_val + 11;
        } else {                           /* higher scales */
            res += gcnt_val;
        }
    }
    return res;
}

/* -------- Cache for memoised sequence values -------- */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t tested_up_to = 0;   /* next integer to test */

/* -------- Grow cache helper -------- */
static void grow_cache ( size_t new_cap )
{
    if ( new_cap <= cache_cap ) return;
    size_t cap = cache_cap ? cache_cap : 16;
    while ( cap < new_cap ) cap <<= 1;
    seq_t *tmp = ( seq_t * )realloc( cache, cap * sizeof(seq_t) );
    if ( !tmp ) abort();
    cache = tmp;
    cache_cap = cap;
}

/* -------- Sequence function --------
   The function returns the n-th term of A162355.
   Callers may request values out of order; the function caches results
   for efficiency.  It is thread‑unsafe but serves the requirement. */
#ifdef __cplusplus
extern "C++" {
#endif
seq_t a ( idx_t n )
{
    if ( n == 0 ) return 0;          /* undefined, falls back */

    /* Ensure the cache holds at least n entries. */
    while ( cache_len < (size_t)n ) {
        seq_t eng = english_uint( tested_up_to );
        seq_t spa = spanish_uint( tested_up_to );
        if ( eng == spa ) {
            grow_cache( cache_len + 1 );
            cache[cache_len++] = (seq_t)tested_up_to;
        }
        ++tested_up_to;
    }
    return cache[(size_t)(n - 1)];
}
#ifdef __cplusplus
}
#endif
