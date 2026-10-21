/*
 * A083234  a(n) = (3*10^n + 2^n)/4.
 * https://oeis.org/A083234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3*10^n + 2^n)/4 Computed using the recurrence a(0)=1,
 *            a(1)=8, a(n)=12*a(n‑1)-20*a(n‑2) (n≥2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* cached terms */
    static idx_t  cache_len = 0;       /* highest index stored +1 */
    static idx_t  cache_cap = 0;       /* allocated capacity */

    /* ensure cache can hold n */
    if (cache_cap <= n) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 2;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* memory exhaustion */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise first two terms */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;           /* a(0) */
        cache[1] = (seq_t)8;           /* a(1) */
        cache_len = 2;
    }

    /* extend cache up to n */
    while (cache_len <= n) {
        seq_t v = (seq_t)12 * cache[cache_len-1]   /* 12*a(n-1) */
                - (seq_t)20 * cache[cache_len-2];  /* 20*a(n-2) */
        cache[cache_len] = v;
        ++cache_len;
    }

    return cache[n];
}
