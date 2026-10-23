/*
 * A025947  Expansion of 1/((1-2*x)*(1-3*x)*(1-7*x)*(1-12*x)).
 * https://oeis.org/A025947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A025947
   Formula: a(n) = 3^(n+1) - 2^(n+1) + 19*a(n-1) - 84*a(n-2),
   a(0) = 1, a(1) = 24
*/
#include <stdlib.h>

static seq_t *cache   = NULL;   /* memoization array of computed values */
static idx_t cache_len = 0;     /* number of entries actually filled */
static size_t cache_cap = 0;    /* allocated capacity of cache          */
static seq_t pow3 = 0;          /* 3^(k+1) for the current last index k */
static seq_t pow2 = 0;          /* 2^(k+1) for the current last index k */

seq_t a(idx_t n)
{
    /* ensure at least the requested index is available */
    if (n < cache_len) {
        return cache[n];
    }

    /* first time: allocate and initialise */
    if (cache == NULL) {
        /* allocate enough for the requested n (at least 2 entries) */
        cache_cap = (n+1) > 2 ? (size_t)(n+1) : 2;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) {
            /* allocation failed – return 0 as a safe fallback */
            return (seq_t)0;
        }
        /* base values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)24;
        cache_len = 2;

        /* pow3 and pow2 correspond to n=1 after the base initialisation */
        pow3 = (seq_t)9; /* 3^(1+1) */
        pow2 = (seq_t)4; /* 2^(1+1) */

        /* if the requested index is 0 or 1, we are done */
        if (n < 2) {
            return cache[n];
        }
    }

    /* enlarge array if necessary */
    if ((size_t)(n+1) > cache_cap) {
        cache_cap = (size_t)(n+1);
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) {
            /* realloc failed – return 0 as a safe fallback */
            return (seq_t)0;
        }
        cache = tmp;
    }

    /* compute any missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* pow3, pow2 currently hold 3^(i-1+1) and 2^(i-1+1) */
        pow3 *= (seq_t)3;   /* now 3^(i+1)   */
        pow2 *= (seq_t)2;   /* now 2^(i+1)   */

        /* recurrence: a(i) = 3^(i+1) - 2^(i+1) + 19*a(i-1) - 84*a(i-2) */
        cache[i] = pow3 - pow2 + (seq_t)19 * cache[i-1] - (seq_t)84 * cache[i-2];
    }
    cache_len = n + 1;
    return cache[n];
}
