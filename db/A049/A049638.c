/*
 * A049638  a(n) = T(n,4), array T as in A049627.
 * https://oeis.org/A049638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + a(n-3) - a(n-5) with initial values: a(0)=2,
 *            a(1)=7, a(2)=9, a(3)=12, a(4)=14, a(5)=18
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 59 (2026-11-03)
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

static seq_t *a_cache = NULL;
static size_t a_cache_cap = 0;
static idx_t a_cache_last = 5;

/* Return the n-th term of A049638 */
seq_t a(idx_t n)
{
    /* Handle the first six terms directly */
    if (n <= 5) {
        switch (n) {
            case 0: return (seq_t)2;
            case 1: return (seq_t)7;
            case 2: return (seq_t)9;
            case 3: return (seq_t)12;
            case 4: return (seq_t)14;
            case 5: return (seq_t)18;
        }
    }

    /* Initialise cache on first use */
    if (a_cache == NULL) {
        ensure_cache(5, &a_cache, &a_cache_cap);
        a_cache[0] = (seq_t)2;
        a_cache[1] = (seq_t)7;
        a_cache[2] = (seq_t)9;
        a_cache[3] = (seq_t)12;
        a_cache[4] = (seq_t)14;
        a_cache[5] = (seq_t)18;
        a_cache_last = 5;
    }

    /* Ensure the cache is large enough for the requested index */
    ensure_cache(n, &a_cache, &a_cache_cap);

    /* If we have already computed up to n, just return it */
    if (n <= a_cache_last)
        return a_cache[n];

    /* Compute missing terms using the recurrence */
    for (idx_t i = a_cache_last + 1; i <= n; ++i) {
        a_cache[i] = a_cache[i - 2] + a_cache[i - 3] - a_cache[i - 5];
    }
    a_cache_last = n;
    return a_cache[n];
}
