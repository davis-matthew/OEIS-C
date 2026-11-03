/*
 * A161586  The list of the k values in the common solutions to the 2
 *          equations 9*k+1=A^2, 13*k+1=B^2.
 * https://oeis.org/A161586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 15 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A161586:
 *  Recurrence:  a(1)=0, a(2)=11, a(3)=1320,
 *                a(n) = 120*(a(n-1)-a(n-2)) + a(n-3)   (n>3)
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

seq_t a(idx_t n)
{
    /* zero or negative indices are ignored */
    if (n <= 1) return 0;
    static seq_t *cache = NULL;          /* array holding computed terms */
    static size_t cache_size = 0;        /* allocated length */
    static idx_t  last = 0;              /* last index that has been computed */

    /* ensure we have room for up to index n */
    ensure_cache(n, &cache, &cache_size);

    /* initialize the first three terms once */
    if (last == 0) {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)11;
        cache[3] = (seq_t)1320;
        last = 3;
    }

    /* if the value is already computed, return it */
    if (n <= last) return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t diff = cache[i - 1] - cache[i - 2];
        cache[i] = (seq_t)120 * diff + cache[i - 3];
    }
    last = n;
    return cache[n];
}
