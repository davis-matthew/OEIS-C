/*
 * A095776  Expansion of (1-9x-27x^3)^(-1/3).
 * https://oeis.org/A095776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=3, a(2)=18;
   for n≥3: n*a(n)=3*(3*n-2)*a(n-1)+27*(n-2)*a(n-3)   (exact division) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;    /* number of values currently filled */
    static size_t cache_cap  = 0;    /* allocated capacity */

    /* Ensure that the cache can hold indices up to n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= (size_t)n + 1)
            new_cap <<= 1;          /* grow sufficiently */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Extend cache if necessary */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)3;
        } else if (i == 2) {
            cache[i] = (seq_t)18;
        } else {
            seq_t ni = (seq_t)i;
            /* coeff1 = 3*(3*n-2), coeff2 = 27*(n-2) */
            seq_t coeff1 = (seq_t)3 * ((seq_t)3 * ni - (seq_t)2);
            seq_t coeff2 = (seq_t)27 * (ni - (seq_t)2);
            seq_t term1 = coeff1 * cache[i - 1];
            seq_t term2 = coeff2 * cache[i - 3];
            seq_t numer = term1 + term2;
            cache[i] = numer / ni;    /* exact division */
        }
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
