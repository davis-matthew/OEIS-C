/*
 * A028021  Expansion of 1/((1-2x)(1-9x)(1-10x)(1-11x)).
 * https://oeis.org/A028021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (28*11^(n+3)-63*10^(n+3)+36*9^(n+3)-2^(n+3))/504
 *            Equivalent linear recurrence: a(n) = 32 a(n‑1) – 359 a(n‑2) +
 *            1588 a(n‑3) – 1980 a(n‑4) with a(0)=1, a(1)=32, a(2)=665,
 *            a(3)=11380
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialize cache with first four terms */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (!cache) return 0;           /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)32;
        cache[2] = (seq_t)665;
        cache[3] = (seq_t)11380;
        cache_len = 4;
    }

    /* If requested term is already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* Grow cache to hold up to term n */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache)
        return 0;                       /* allocation failure */
    cache = new_cache;

    /* Compute terms from cache_len up to n */
    for (idx_t k = cache_len; k <= n; ++k) {
        /* a(k) = 32*a(k-1) - 359*a(k-2) + 1588*a(k-3) - 1980*a(k-4) */
        cache[k] = (seq_t)32 * cache[k - 1]
                 - (seq_t)359 * cache[k - 2]
                 + (seq_t)1588 * cache[k - 3]
                 - (seq_t)1980 * cache[k - 4];
    }
    cache_len = n + 1;
    return cache[n];
}
