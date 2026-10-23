/*
 * A167816  Numerator of x(n) = x(n-1) + x(n-2), x(0)=0, x(1)=1/3;
 *          denominator=A167817.
 * https://oeis.org/A167816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 48, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167816: numerator of the recurrence
   x(n) = x(n-1) + x(n-2),  x(0)=0,  x(1)=1/3.
   The numerators satisfy the linear 8‑term recurrence
   a(n) = 7*a(n-4) - a(n-8)   for n >= 8,
   with initial values:
   a(0)=0,a(1)=1,a(2)=1,a(3)=2,a(4)=1,a(5)=5,a(6)=8,a(7)=13.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of computed elements */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* ensure capacity for at least n+1 elements */
    if (cache == NULL) {
        cache_cap = (n+1 > 8) ? n+1 : 8;          /* at least 8 slots */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;     /* out of memory — unlikely */
    } else if ((size_t)(n+1) > cache_cap) {
        /* grow by doubling */
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)(n+1)) new_cap = n+1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the first 8 terms only once */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)1;
        cache[5] = (seq_t)5;
        cache[6] = (seq_t)8;
        cache[7] = (seq_t)13;
        cache_len = 8;
    }

    /* if requested n is already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* compute terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i < 8) {
            /* already set in the initialisation. */
            val = cache[i];
        } else {
            /* a(i) = 7*a(i-4) - a(i-8) */
            val = 7 * cache[i - 4] - cache[i - 8];
        }
        cache[i] = val;
    }
    cache_len = n + 1;

    return cache[n];
}
