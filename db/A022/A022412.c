/*
 * A022412  Expansion of 1/((1-x)(1-5x)(1-6x)(1-11x)).
 * https://oeis.org/A022412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: The sequence a(n) is given by the recurrence
 * a(0) = 1, a(1) = 23, and for n ≥ 2:
 *   a(n) = 17*a(n-1) - 66*a(n-2) + (5^(n+1) - 1)/4
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t len = 0;   /* number of elements stored in cache */
    static size_t cap = 0;   /* allocated capacity */
    static seq_t pow5 = 0;   /* 5^(current_index+1) for the next term to compute */
    static bool pow5_ready = false;

    /* ensure cache is initialized */
    if (len == 0) {
        /* allocate space for two initial terms */
        cap = 2;
        cache = seq_alloc(cap);
        cache[0] = 1;  /* a(0) */
        cache[1] = 23; /* a(1) */
        len = 2;
        /* pow5 for n = 2 : 5^(2+1) = 5^3 */
        pow5 = ipow((seq_t)5, (seq_t)3);
        pow5_ready = true;
    }

    /* if requested index is already cached, return it */
    if ((idx_t)len > n) {
        return cache[n];
    }

    /* extend cache up to n */
    for (idx_t k = len; k <= n; ++k) {
        /* For k == 0 or k == 1, values are already set, skip. */
        if (k == 0) {
            /* nothing to do */
        } else if (k == 1) {
            /* nothing to do */
        } else {
            /* Compute a(k) using the recurrence */
            seq_t term = 0;

            /* 17*a(k-1) */
            term += cache[k - 1] * 17;

            /* -66*a(k-2) */
            term -= cache[k - 2] * 66;

            /* + (5^(k+1) - 1)/4, pow5 holds 5^(k+1) for current k */
            term += (pow5 - 1) / 4;

            /* store result */
            if (len == cap) {
                /* grow capacity */
                size_t new_cap = cap * 2;
                seq_t *new_cache = seq_realloc(cache, cap, new_cap);
                cache = new_cache;
                cap = new_cap;
            }
            cache[len] = term;
            len++;
        }

        /* update pow5 for the next iteration (to 5^(k+2)) */
        if (pow5_ready) {
            pow5 = pow5 * 5;
        }
    }

    return cache[n];
}
