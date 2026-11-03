/*
 * A015196  Sum of Gaussian binomial coefficients for q=10.
 * https://oeis.org/A015196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Validated: standard, pass, 29 terms through n = 55 (2026-11-03)
 * Helpers:   ipow_u64_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) + (10^(n-1)-1)*a(n-2),  a(0)=1, a(1)=2 */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_u64_idx.h"
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;

    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Make sure we can store indices up to n */
    ensure_cache(n, &cache, &cap);

    /* Initialize first two terms if not already done */
    if (!cache[0]) cache[0] = (seq_t)1;
    if (!cache[1]) cache[1] = (seq_t)2;

    /* Compute missing terms up to n */
    for (idx_t i = 2; i <= n; ++i) {
        if (cache[i]) continue;   /* already computed */
        seq_t pow10 = ipow_u64_idx(10, i - 1);
        seq_t term1 = cache[i - 1] * 2;
        seq_t term2 = (pow10 - 1) * cache[i - 2];
        cache[i] = term1 + term2;
    }

    return cache[n];
}
