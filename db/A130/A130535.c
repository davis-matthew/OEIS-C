/*
 * A130535  a(0)=a(1)=1; a(n+2) = a(floor((n+1)/a(n+1))) + a(floor(n/a(n))).
 * https://oeis.org/A130535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   not stated
 * Range:     u8 n <= 405, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9406 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Oeis sequence A130535:
   a(0) = 1; a(1) = 1;
   a(n+2) = a( floor((n+1)/a(n+1)) ) + a( floor(n/a(n)) )
   (for n ≥ 0)  */
#include <stddef.h>
#include <stdint.h>
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
    /* static cache and bookkeeping */
    static seq_t *cache = NULL;      /* array of computed values */
    static size_t  cap   = 0;        /* current allocated length */
    static idx_t   last  = 1;        /* highest index computed so far */

    /* initialise cache with a[0] and a[1] on first call */
    if (!cache) {
        ensure_cache(1, &cache, &cap);   /* allocate at least two elements */
        cache[0] = 1;
        cache[1] = 1;
    }

    /* return directly for small indices */
    if (n <= 1) {
        return 1;
    }

    /* grow cache to hold indices up to n */
    if (n >= cap) {
        ensure_cache(n, &cache, &cap);
    }

    /* compute missing values in ascending order */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* i corresponds to n+2 in the recurrence, so we need
           a[i-1] and a[i-2] which are already computed. */
        idx_t idx1 = (idx_t)(((seq_t)(i - 1)) / cache[i - 1]);  /* floor((i-1)/a[i-1]) */
        idx_t idx2 = (idx_t)(((seq_t)(i - 2)) / cache[i - 2]);  /* floor((i-2)/a[i-2]) */
        cache[i] = cache[idx1] + cache[idx2];
    }

    /* update bookkeeping */
    if (n > last) {
        last = n;
    }

    return cache[n];
}
