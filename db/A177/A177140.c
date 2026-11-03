/*
 * A177140  Expansion of (1 + 4*x - x^2 - 2*x^3)/(1 - 3*x - 14*x^2 + 15*x^3 +
 *          7*x^4).
 * https://oeis.org/A177140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 23 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A177140 – Expansion of (1 + 4*x - x^2 - 2*x^3)/(1 - 3*x - 14*x^2 + 15*x^3 + 7*x^4)
 *
 *  Recurrence (n ≥ 5):
 *      a(n) = 3*a(n-1) + 14*a(n-2) – 15*a(n-3) – 7*a(n-4)
 *  with initial values
 *      a(1) = 1,  a(2) = 7,  a(3) = 34,  a(4) = 183.
 *
 *  The function caches computed values so calls can be in any order.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Allocate and free using malloc/free when compiled as C. */
static seq_t *alloc_cache(idx_t n)
{
    return mem_new(seq_t, n);
}

/* Helper: free an array allocated by alloc_seq. */
static void free_seq(seq_t *p)
{
    mem_free(p);
}

seq_t a(idx_t n)
{
    /*  For n < 1 the behaviour is undefined; return 0 to keep the type */
    if (n == 0) return (seq_t)0;

    static seq_t *cache = NULL;   /* array of a(1)…a(cached_len) */
    static idx_t cached_len = 0;  /* number of stored terms   */

    /* If we need more terms, extend the cache */
    if (n > cached_len) {
        idx_t new_len = n;
        seq_t *new_cache = alloc_cache(new_len);

        /* copy the old cache contents if any */
        for (idx_t i = 0; i < cached_len; ++i)
            new_cache[i] = cache[i];

        /* initialise the first four terms (they may be newly allocated) */
        if (new_len >= 1) new_cache[0] = (seq_t)1;      /* a(1) */
        if (new_len >= 2) new_cache[1] = (seq_t)7;      /* a(2) */
        if (new_len >= 3) new_cache[2] = (seq_t)34;     /* a(3) */
        if (new_len >= 4) new_cache[3] = (seq_t)183;    /* a(4) */

        /* compute the recurrence for indices ≥ 4 (a(5), a(6), …) */
        idx_t start = cached_len;
        if (start < 4) start = 4;            /* need at least four values first */
        for (idx_t i = start; i < new_len; ++i) {
            new_cache[i] =
                3 * new_cache[i-1] + 14 * new_cache[i-2]
                - 15 * new_cache[i-3] - 7 * new_cache[i-4];
        }

        /* free the old cache (allocated by alloc_cache) */
        if (cache) free_seq(cache);

        cache      = new_cache;
        cached_len = new_len;
    }

    /*  Return the requested term  */
    return cache[n-1];   /* cache is 0‑based: cache[0] = a(1) */
}
