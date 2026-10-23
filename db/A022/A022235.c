/*
 * A022235  Gaussian binomial coefficients [ n,6 ] for q = 7.
 * https://oeis.org/A022235
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..6} (7^(n-i+1)-1)/(7^i-1)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence used: a(n) = a(n-1) * (7^n-1)/(7^(n-6)-1)  */
#include <stdlib.h>

static seq_t *a_cache = NULL;            /* cache of computed terms  */
static idx_t  a_cache_max = 0;           /* highest index computed   */
static seq_t  pow7  = 0;                 /* current 7^n              */
static seq_t  pow7m6= 0;                 /* current 7^(n-6)          */

seq_t a(idx_t n)
{
    if (n < 6)            /* behaviour below offset is undefined */
        return (seq_t)0;

    /* first call: initialise cache up to n = 6 */
    if (a_cache == NULL) {
        a_cache_max = 6;
        a_cache = (seq_t *)malloc((a_cache_max + 1) * sizeof(seq_t));
        if (a_cache == NULL) return (seq_t)0;     /* allocation failed */

        a_cache[6] = (seq_t)1;                          /* a(6) = 1  */
        /* initialise powers:
           pow7  = 7^6
           pow7m6 = 7^(6-6) = 1
        */
        pow7    = (seq_t)1;
        pow7m6  = (seq_t)1;
        for (idx_t i = 0; i < 6; ++i) {
            pow7 *= (seq_t)7;      /* 7^6 */
            /* pow7m6 stays 1 for i=6 (since 7^0) */
        }
    }

    /* extend cache if required */
    if (n > a_cache_max) {
        idx_t new_max = n;
        seq_t *new_cache = (seq_t *)realloc(a_cache, (new_max + 1) * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0;  /* reallocation failed */

        a_cache = new_cache;

        /* compute terms from a_cache_max + 1 up to n */
        for (idx_t i = a_cache_max + 1; i <= new_max; ++i) {
            /* update powers */
            pow7   *= (seq_t)7;          /* 7^i   */
            pow7m6 *= (seq_t)7;          /* 7^(i-6) */

            /* recurrence:
               a(i) = a(i-1) * (pow7 - 1) / (pow7m6 - 1)
            */
            seq_t num  = a_cache[i - 1] * (pow7 - (seq_t)1);
            seq_t den  = pow7m6   - (seq_t)1;
            a_cache[i] = num / den;
        }
        a_cache_max = new_max;
    }

    return a_cache[n];
}
