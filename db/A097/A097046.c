/*
 * A097046  a(n) = a(n-1) + a(floor(sqrt(n))) for n > 1; a(1) = 1.
 * https://oeis.org/A097046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(floor(sqrt(n))), a(1) = 1
 * Range:     u8 n <= 46, u16 n <= 982, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Integer floor square root for idx_t (unsigned 64‑bit). */
static idx_t floor_sqrt(idx_t n)
{
    if (n == 0 || n == 1)
        return n;
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid > 0 && mid > n / mid) {
            high = mid - 1;
        } else {
            if ((mid + 1) > 0 && (mid + 1) > n / (mid + 1))
                return mid;
            low = mid + 1;
        }
    }
    return low - 1;
}

/* Memoised computation of A097046. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* 1‑based array, cache[0] unused */
    static idx_t cache_len = 0;   /* highest index stored (>=1 if cache non‑empty) */

    if (n == 0)   /* undefined in OEIS, but return 0 for safety */
        return 0;

    /* Expand cache if needed. */
    if (n > cache_len) {
        idx_t new_len = n;
        seq_t *new_cache = (seq_t*)realloc(cache, (new_len + 1) * sizeof(seq_t));
        if (!new_cache) {
            /* malloc failure – fallback to recomputation each call */
            retreac: ;
        }
        cache = new_cache;
        /* Initialize from previous size + 1 to new_len */
        if (cache_len == 0) {
            cache[0] = 0;            /* unused */
            cache[1] = (seq_t)1;     /* a(1) */
            cache_len = 1;
        }
        for (idx_t i = cache_len + 1; i <= new_len; ++i) {
            idx_t k = floor_sqrt(i);
            cache[i] = cache[i - 1] + cache[k];
        }
        cache_len = new_len;
    }
    return cache[n];
}
