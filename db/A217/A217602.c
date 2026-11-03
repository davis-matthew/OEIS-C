/*
 * A217602  Let a(1) = 1. If n and a(n-1) have the same parity, a(n) = n +
 *          a(n-1), otherwise a(n) = abs(n - a(n-1)).
 * https://oeis.org/A217602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; for n>1, a(n)= n+a(n-1) if parity(n)=parity(a(n-1)),
 *            else a(n)=|n-a(n-1)|
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 85 terms through n = 986 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

/* External linkage function computing OEIS A217602 */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* cache[0] unused */
    static size_t cap = 0;
    static idx_t computed = 0;        /* highest index already computed */

    if (n == 0) return 0;            /* safety, offset starts at 1 */

    if (n > computed) {
        /* Ensure the cache can hold indices up to n */
        ensure_cache(n, &cache, &cap);

        for (idx_t i = computed + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = (seq_t)1;
            } else {
                seq_t prev = cache[i - 1];
                seq_t curr;
                /* check parity of i and prev */
                if ((i & 1ULL) == (prev & 1ULL)) {
                    curr = prev + (seq_t)i;
                } else {
                    curr = (prev >= (seq_t)i) ? prev - (seq_t)i
                                              : (seq_t)i - prev;
                }
                cache[i] = curr;
            }
        }
        computed = n;
    }

    return cache[n];
}
