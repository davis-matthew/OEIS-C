/*
 * A190865  E.g.f. exp(x+x^3/6).
 * https://oeis.org/A190865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*sum_{k=0..⌊n/3⌋} 1/(k!*(n-3k)!*6^k), recurrence:
 *            2*a(n) = 2*a(n-1) + (n-2)*(n-1)*a(n-3), base: a(0)=a(1)=a(2)=1
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 29, u128 n <= 49,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 663 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
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

seq_t a(idx_t n)
{
    /* cache holds computed values 0..(cached_last-1) */
    static seq_t *cache  = NULL;   /* cache pointer */
    static size_t cap   = 0;       /* allocated capacity (number of seq_t cells) */
    static size_t last  = 0;       /* index of first element not yet computed */

    /* Make sure there is space for indices 0..n */
    if (n + 1 > cap)
        ensure_cache(n + 1, &cache, &cap);

    /* Compute from last up to n */
    for (idx_t i = last; i <= n; ++i) {
        if (i <= 2) {
            /* base values */
            cache[i] = (seq_t)1;
        } else {
            /* recurrence: 2*a(i) = 2*a(i-1) + (i-2)*(i-1)*a(i-3) */
            seq_t term  = cache[i - 1];
            term += term;                        /* 2*a(i-1) */
            seq_t mult = ((seq_t)(i - 2)) * ((seq_t)(i - 1));
            mult *= cache[i - 3];
            term += mult;
            cache[i] = term / 2;                /* exact division by 2 */
        }
    }

    last = n + 1;            /* next time, we start from here */
    return cache[n];
}
