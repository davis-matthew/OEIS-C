/*
 * A050040  a(n) = a(n-1) + a(m) for n >= 4, where m = 2*n - 3 - 2^(p+1) and p
 *          is the unique integer such that 2^p < n - 1 <= 2^(p+1), starting
 *          with a(1) = 1, a(2) = 2, and a(3) = 1.
 * https://oeis.org/A050040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(m) for n >= 4, where m = 2*n - 3 - 2^(p+1) and
 *            p is the unique integer such that 2^p < n-1 <= 2^(p+1). Initial
 *            values: a(1)=1, a(2)=2, a(3)=1
 * Range:     u8 n <= 25, u16 n <= 59, u32 n <= 197, u64 n <= 771,
 *            u128 n <= 3649, big any n
 * Helpers:   alloc_memory, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses memoization and computes values from 1
   up to the requested n, storing them in a dynamic array. */

#include <stddef.h>
#include <stdarg.h>
#include <assert.h>
#include "../utils/alloc_memory.h"   /* for allocate_row, etc. */
#include "../utils/ensure_cache.h" /* ensures cache capacity */

/* Forward declaration of helper to compute the floor of log2 */
static inline int floor_log2_idx(idx_t x)
{
    /* Return floor(log2(x)) for x > 0; otherwise return -1. */
    int r = 0;
    while (x > 1) {
        x >>= 1;
        r++;
    }
    return r;
}

/* External linkage function required by the framework */
seq_t a(idx_t n)
{
    /* Static cache and bookkeeping */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated size */
    static idx_t computed_max = 0;  /* highest index computed so far */

    /* Ensure cache can hold index n */
    ensure_cache(n, &cache, &cap);

    /* If we already have the value, return it */
    if (computed_max >= n)
        return cache[n];

    /* Compute values up to n iteratively */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        if (i == 1)
            cache[i] = (seq_t)1;
        else if (i == 2)
            cache[i] = (seq_t)2;
        else if (i == 3)
            cache[i] = (seq_t)1;
        else {
            /* Determine p: 2^p < i-1 ≤ 2^(p+1)  →  p = floor(log2(i-2)) */
            int p = floor_log2_idx(i - 2);
            /* 2^(p+1) fits in idx_t for all relevant n (here n ≤ 10000) */
            idx_t pow2p1 = ((idx_t)1) << (p + 1);
            idx_t m = (idx_t)(2 * i - 3 - pow2p1);
            /* m < i by definition, so cache[m] is already computed */
            cache[i] = cache[i - 1] + cache[m];
        }
    }

    computed_max = n;
    return cache[n];
}
