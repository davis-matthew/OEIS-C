/*
 * A062282  Number of permutations of n elements with an even number of fixed
 *          points.
 * https://oeis.org/A062282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) - (-2)^(n-1), a(0) = 1 (computed iteratively
 *            with a cache of previous terms)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_max = (idx_t)-1;          /* no terms cached yet */
    /* state of the power (-2)^e, with e = current exponent used in next step */
    static seq_t pow_abs = 1;   /* |(-2)^e|  = 2^e */
    static int sign = 1;        /* (-1)^e  :  1 for e even, -1 for e odd */
    static idx_t exp_e = 0;     /* exponent e currently represented by pow_abs and sign */

    /* initialise cache on first call */
    if (cache_max == (idx_t)-1) {
        cache_cap = 8;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        cache_max = 0;
    }

    /* grow cache if necessary */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap;
        while ((idx_t)new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t term;
        /* e = i-1 is the exponent used for this step */
        if (sign == 1) {                   /* (-2)^(i-1) = + 2^(i-1)  */
            term = (seq_t)i * cache[i-1] - pow_abs;
        } else {                           /* (-2)^(i-1) = - 2^(i-1)  */
            term = (seq_t)i * cache[i-1] + pow_abs;
        }

        cache[i] = term;

        /* update power for next exponent (i) */
        pow_abs = pow_abs * (seq_t)2;      /* 2^(i)  */
        sign = -sign;                     /* alternate sign   */
        exp_e++;                           /* not used directly but kept for clarity */
    }

    cache_max = n;
    return cache[n];
}
