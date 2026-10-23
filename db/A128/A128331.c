/*
 * A128331  a(1)=1. a(n) = number of positive numbers <= n that are coprime to
 *          a(n-1).
 * https://oeis.org/A128331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>1, a(n) = #{k ≤ n : gcd(k , a(n−1)) = 1}
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* static cache: cache[0] unused, cache[1..cached_up_to] hold the sequence */
static seq_t *cache = NULL;
static idx_t   cached_up_to = 0;

/* Allocate or enlarge the cache to hold indices up to n (inclusive)   */
static void
ensure_cache(idx_t n)
{
    if (n <= cached_up_to)
        return;                 /* already computed */

    /* size of new array: index 0 unused, so +1 */
    size_t new_cap = (size_t)n + 1;

    /* allocate new array: use new[] in C++ and malloc in C */
#ifdef __cplusplus
    seq_t *new_cache = new seq_t[new_cap];
#else
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) abort();
#endif

    /* copy old values */
    for (idx_t i = 1; i <= cached_up_to; ++i)
        new_cache[i] = cache[i];

    /* free old array */
    if (cache != NULL) {
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
    }

    cache = new_cache;

    /* compute missing terms */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        if (i == 1) {                /* a(1) = 1 */
            cache[i] = 1;
            continue;
        }
        seq_t prev = cache[i - 1];
        seq_t cnt  = 0;

        for (idx_t k = 1; k <= i; ++k) {
            if (gcd((seq_t)k, prev) == 1)
                ++cnt;
        }
        cache[i] = cnt;
    }

    cached_up_to = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for n < 1 */
    ensure_cache(n);
    return cache[n];
}
