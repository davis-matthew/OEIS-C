/*
 * A140492  Trajectory of 3 under repeated application of the map: n -> n +
 *          third-smallest number that does not divide n.
 * https://oeis.org/A140492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-12) - a(n-13) for n > 24, a(1) = 3, a(n) =
 *            a(n-1) + third_non_divisor(a(n-1)) for 2 ≤ n ≤ 24
 * Range:     u8 n <= 52, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t, NULL */
#include <stdlib.h>   /* malloc, realloc, abort */

static inline seq_t third_non_divisor(seq_t x)
{
    seq_t d   = 1;
    seq_t cnt = 0;
    while (cnt < 3) {
        if (x % d != 0) {
            ++cnt;
            if (cnt == 3) break;
        }
        ++d;
    }
    return d;
}

static void ensure_cache(idx_t n, seq_t **arr, size_t *cap)
{
    if (*cap <= (size_t)n) {
        size_t new_cap = (*cap == 0) ? 32 : (*cap * 2);
        while (new_cap <= (size_t)n) new_cap *= 2;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (*arr == NULL) abort();
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined below the offset */

    static seq_t *seq_cache = NULL;
    static size_t   cache_cap = 0;
    static idx_t   cache_last = 0;

    /* First call: allocate cache and compute the first 24 terms. */
    if (seq_cache == NULL) {
        ensure_cache(24, &seq_cache, &cache_cap);
        seq_cache[0] = 0;          /* unused placeholder */
        seq_cache[1] = 3;
        for (idx_t i = 2; i <= 24; ++i)
            seq_cache[i] = seq_cache[i-1] + third_non_divisor(seq_cache[i-1]);

        cache_last = 24;
    }

    /* Return immediately if the requested term is already cached. */
    if (n <= cache_last) return seq_cache[n];

    /* Grow the cache to accommodate the requested index. */
    ensure_cache(n, &seq_cache, &cache_cap);

    /* Compute missing terms up to n. */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i <= 24) {
            seq_cache[i] = seq_cache[i-1] + third_non_divisor(seq_cache[i-1]);
        } else {
            seq_cache[i] = seq_cache[i-1] + seq_cache[i-12] - seq_cache[i-13];
        }
    }
    cache_last = n;
    return seq_cache[n];
}
