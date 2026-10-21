/*
 * A085523  a(n) = Product_{k=0..n} (4^(2k+1)+1).
 * https://oeis.org/A085523
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=0..n} (4^(2k+1)+1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for already computed values */
    static idx_t last_k         = -1;      /* highest k that has been processed   */
    static seq_t last_prod = 0;            /* a(last_k)                            */
    static seq_t last_pow16   = 0;          /* 16^(last_k+1)  (starting 1 for k=0)  */

    static seq_t *cache = NULL;            /* array: cache[i] = a(i-1), i>=0
                                            * i = 0 -> a(-1) = 1
                                            */
    static size_t cache_len = 0;           /* number of values stored */
    static size_t cache_cap = 0;           /* capacity of cache array */

    /* initialization on first call */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_len = 1;                     /* a(-1) */
        cache[0] = (seq_t)1;                /* a(-1) = 1 */
        last_k = -1;
        last_prod = (seq_t)1;
        last_pow16 = (seq_t)1;              /* 16^(last_k+1) = 16^0 = 1   */
    }

    /* negative indexes below -1 are undefined; return 1 */
    if (n < -1) return (seq_t)1;

    /* index in cache array: idx = n+1 (so a(-1) -> 0, a(0) -> 1, ...) */
    size_t idx = (size_t)(n + 1);

    /* if requested value already computed, return it */
    if (idx < cache_len)
        return cache[idx];

    /* otherwise compute successive terms up to requested n */
    while (last_k < n) {
        /* compute next multiplier: 4*16^k + 1  where k = last_k+1 */
        seq_t term = (seq_t)4 * last_pow16 + (seq_t)1;
        last_prod = last_prod * term;   /* a(k) = a(k-1) * term */
        last_k++;                       /* move to new k */

        /* store new value in cache */
        if (cache_len == cache_cap) {
            cache_cap *= 2;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = last_prod;

        /* update pow16 for next iteration: multiply by 16 */
        last_pow16 = last_pow16 * (seq_t)16;
    }

    return cache[idx];
}
