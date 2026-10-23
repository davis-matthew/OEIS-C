/*
 * A028162  Expansion of 1/((1-4x)(1-9x)(1-10x)(1-12x)).
 * https://oeis.org/A028162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A028162: Expansion of 1/((1-4x)(1-9x)(1-10x)(1-12x))
   Formula used: a(n) = 22*a(n‑1) - 120*a(n‑2) + (9^(n+1) - 4^(n+1))/5,
   with a(0) = 1, a(1) = 35.  This implementation stores computed terms
   in a static cache and expands it lazily.  It works for any seq_t type
   provided by the build environment. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache state */
    static seq_t *cache   = NULL;   /* array of computed terms     */
    static size_t cache_len = 0;    /* number of terms currently stored */
    static size_t cache_cap = 0;    /* capacity of cache array       */
    static seq_t pow4 = (seq_t)1;   /* 4^k where k = cache_len      */
    static seq_t pow9 = (seq_t)1;   /* 9^k where k = cache_len      */

    /* helper to grow the cache array */
    if (cache_cap < (size_t)(cache_len + 1)) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 1;
        while (new_cap < cache_len + 1) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* if the requested index is already available, return it */
    if (cache_len > (size_t)n)
        return cache[(size_t)n];

    /* compute the missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* ensure there is space for the new term */
        if (cache_cap <= i) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 1;
            while (new_cap <= i) new_cap <<= 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        if (i == 0) {
            cache[0] = (seq_t)1;
            /* advance powers for the next index */
            pow4 = pow4 * (seq_t)4;   /* pow4 = 4^1 */
            pow9 = pow9 * (seq_t)9;   /* pow9 = 9^1 */
            cache_len = 1;
        } else if (i == 1) {
            cache[1] = (seq_t)35;
            pow4 = pow4 * (seq_t)4;   /* pow4 = 4^2 */
            pow9 = pow9 * (seq_t)9;   /* pow9 = 9^2 */
            cache_len = 2;
        } else {
            seq_t nextpow4 = pow4 * (seq_t)4;   /* 4^(i+1) */
            seq_t nextpow9 = pow9 * (seq_t)9;   /* 9^(i+1) */
            seq_t term = (nextpow9 - nextpow4) / (seq_t)5;

            cache[i] = (seq_t)22 * cache[i-1]
                     - (seq_t)120 * cache[i-2]
                     + term;

            pow4 = nextpow4;   /* update to 4^(i+1) for the next iteration */
            pow9 = nextpow9;
            cache_len = i + 1;
        }
    }

    return cache[(size_t)n];
}
