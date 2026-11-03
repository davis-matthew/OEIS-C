/*
 * A165425  a(1) = 1, a(2) = 7, a(n) = product of the previous terms for n >=
 *          3.
 * https://oeis.org/A165425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{i=1}^{n-1} a(i) with a(1) = 1, a(2) = 7, n >= 3
 *            (equivalently, a(3)=7 and a(n)=a(n-1)^2 for n>=4)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Validated: standard, pass, 13 terms through n = 13 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
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

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* behaviour below offset */

    if (n == 1)
        return (seq_t)1;

    if (n == 2 || n == 3)
        return (seq_t)7;

    seq_t val = (seq_t)7;
    /* a(4) = 7^2, a(5) = 7^4, a(6) = 7^8, ... */
    for (idx_t i = 4; i <= n; ++i)
        val = val * val;

    return val;
}
