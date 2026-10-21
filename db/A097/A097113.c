/*
 * A097113  Expansion of (1 + 5*x - 12*x^2 - 80*x^3)/(1 - 33*x^2 + 272*x^4).
 * https://oeis.org/A097113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 33*a(n-2) - 272*a(n-4); a(0)=1, a(1)=5, a(2)=21, a(3)=85
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache      = NULL;
    static idx_t cache_len   = 0;
    static const idx_t INIT   = 4;          /* number of initial terms              */

    /* create the cache if empty */
    if (cache_len == 0) {
        cache_len = INIT;
        cache = (seq_t *)calloc((size_t)cache_len, sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* out‑of‑memory, return 0 as a safety net */

        /* initialise the first four terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)21;
        cache[3] = (seq_t)85;
    }

    /* extend the cache up to n */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out‑of‑memory, safety return */
        cache = tmp;

        /* compute new terms one by one */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 4) {
                /* already initialized */
                continue;
            }
            cache[i] = ( (seq_t)33 * cache[i-2] ) - ( (seq_t)272 * cache[i-4] );
        }
        cache_len = new_len;
    }

    return cache[n];
}
