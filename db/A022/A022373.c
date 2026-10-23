/*
 * A022373  Fibonacci sequence beginning 2, 22.
 * https://oeis.org/A022373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 2, a(1) = 22
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of elements stored (≥ 2 after init) */
static idx_t cache_cap = 0;   /* allocated capacity */

/* Computes the A022373 sequence (Fibonacci variant starting with 2,22). */
seq_t a(idx_t n)
{
    if (cache == NULL) {
        /* Ensure at least space for the first two terms. */
        cache_cap = (n + 1) < 2 ? 2 : (idx_t)(n + 1);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 2;
        cache[1] = 22;
        cache_len = 2;
    }

    if (n >= cache_len) {
        idx_t needed_len = n + 1;

        if (needed_len > cache_cap) {
            idx_t new_cap = needed_len * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* Allocation failure: return 0 as a safe fallback. */
                return 0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = cache[i - 1] + cache[i - 2];
        }
        cache_len = needed_len;
    }

    return cache[n];
}
