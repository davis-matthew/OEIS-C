/*
 * A170217  Number of reduced words of length n in Coxeter group on 16
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 16, a(n) = 16*15^(n-1) for 1 ≤ n < 40, a(n) =
 *            14*∑_{k=1}^{39} a(n-k) – 105*a(n-40) for n ≥ 40 (OEIS A170217)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    size_t needed = (size_t)n + 1;
    if (needed <= cache_size)   /* already computed */
        return cache[(size_t)n];

    /* expand the cache */
    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[needed];
    for (size_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];
    delete[] cache;
#else
    new_cache = (seq_t*)realloc(cache, needed * sizeof(seq_t));
#endif
    cache = new_cache;

    /* compute missing entries */
    for (size_t i = cache_size; i < needed; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)16;
        } else if (i <= 39) {
            cache[i] = cache[i - 1] * (seq_t)15;
        } else {
            seq_t sum = 0;
            for (size_t k = 1; k <= 39; ++k)
                sum += cache[i - k];
            cache[i] = sum * (seq_t)14 - cache[i - 40] * (seq_t)105;
        }
    }

    cache_size = needed;
    return cache[(size_t)n];
}
