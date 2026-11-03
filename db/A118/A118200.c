/*
 * A118200  Start with 1 and repeatedly reverse the digits and add 66 to get
 *          the next term.
 * https://oeis.org/A118200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less, easy
 * Formula:   a(n) = reverse_decimal(a(n-1)) + 66 with a(1) = 1. The sequence
 *            is memoised
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-03)
 * Helpers:   mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/reverse_digits.h"
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
    /*  tmp[1] holds a(1) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_max = 0;

    if (n < 1)
        return 0;   /* undefined below offset, but return 0 */

    if (cache == NULL) {
        ensure_cache(1, &cache, &cache_cap);   /* reserve space for index 1 */
        cache[1] = (seq_t)1;
        cache_max = 1;
    }

    if (n > cache_max) {
        /* extend cache to reach index n */
        ensure_cache(n, &cache, &cache_cap);
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            seq_t rev_val = reverse_digits(cache[i - 1]);
            cache[i] = rev_val + (seq_t)66;
        }
        cache_max = n;
    }

    return cache[n];
}
