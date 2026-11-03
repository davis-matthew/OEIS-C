/*
 * A043438  Numbers having two 4's in base 8.
 * https://oeis.org/A043438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑8 representation contains exactly two digits
 *            equal to 4
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
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

static int count_4_oct(seq_t x)
{
    int c = 0;
    while (x > 0) {
        if ((x % 8) == 4)
            ++c;
        x /= 8;
    }
    return c;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t current_val = 0;

    if (n == 0)
        return 0;               /* not used, offset is 1 */

    if (n <= cache_len)
        return cache[n - 1];

    /* ensure the cache can hold at least n terms */
    ensure_cache((idx_t)n, &cache, &cache_cap);

    for (seq_t x = current_val + 1;; ++x) {
        if (count_4_oct(x) == 2) {
            cache[cache_len++] = x;
            current_val = x;
            if (cache_len == n)
                return x;
        }
    }
}
