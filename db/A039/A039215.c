/*
 * A039215  Numbers whose base-12 representation has the same number of 0's
 *          and 6's.
 * https://oeis.org/A039215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n‑th positive integer whose base‑12 representation contains
   the same number of 0's and 6's. */
#include <stdlib.h>
#include <stdbool.h>

/* Compute the n-th member of OEIS A039215.  n ≥ 1. */
seq_t a(idx_t n)
{
    /* Static cache (memoisation). */
    static seq_t    *cache = NULL;      /* array of found terms        */
    static size_t   cache_cap = 0;      /* current capacity of cache   */
    static size_t   cache_len = 0;      /* number of terms currently   */
    static seq_t    next_candidate = 1; /* next integer to test        */

    /* If n is zero (outside offset), return 0 – should not happen. */
    if (n == 0) return 0;

    /* Grow cache until it contains the required nth term. */
    while (cache_len < (size_t)n) {
        seq_t v = next_candidate;
        seq_t x = v;
        unsigned c0 = 0, c6 = 0;

        /* Count digits 0 and 6 in base‑12 representation. */
        while (x > (seq_t)0) {
            unsigned d = (unsigned)(x % (seq_t)12);
            if (d == 0)   c0++;
            else if (d == 6) c6++;
            x /= (seq_t)12;
        }

        if (c0 == c6) {
            /* Ensure capacity for the new term. */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                if (new_cap < cache_len + 1) new_cap = cache_len + 1;
                /* cast to seq_t* for C++ compatibility */
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = v;
        }

        next_candidate++;   /* move to the next integer */
    }

    return cache[n - 1];
}
