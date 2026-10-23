/*
 * A025239  a(n) = a(1)*a(n-1) + a(2)*a(n-2) + ...+ a(n-2)*a(2) for n >= 3.
 * https://oeis.org/A025239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2, a(2) = 3, a(n) = a(1)*a(n-1) + a(2)*a(n-2) + … +
 *            a(n-2)*a(2) (n ≥ 3)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 30, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Dynamic cache for previously computed values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure `cache` has room for indices up to `needed` (inclusive). */
static void ensure_cache_size(idx_t needed)
{
    /* cache_cap is the allocated size; we use indices 0..cache_cap-1 */
    if (cache_cap <= (size_t)needed) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        if (new_cap <= (size_t)needed) {
            new_cap = (size_t)needed + 1;
        }
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        if (cache) {
            memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
            free(cache);
        }
        memset(new_cache + cache_cap,
               0, (new_cap - cache_cap) * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n < 1 */

    /* Initialise or grow the cache */
    if (!cache) {
        ensure_cache_size(2);
        cache[1] = 2;
        cache[2] = 3;
    } else {
        ensure_cache_size(n);
        if (cache[1] == 0) cache[1] = 2;
        if (cache[2] == 0) cache[2] = 3;
    }

    if (n == 1) return cache[1];
    if (n == 2) return cache[2];

    /* Compute values up to `n` as needed */
    for (idx_t i = 3; i <= n; ++i) {
        if (cache[i] != 0) continue;           /* already computed */
        seq_t sum = 0;
        for (idx_t k = 1; k <= i - 2; ++k) {
            sum += cache[k] * cache[i - k];
        }
        cache[i] = sum;
    }
    return cache[n];
}
