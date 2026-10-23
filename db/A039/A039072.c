/*
 * A039072  Numbers whose base-8 representation has the same number of 4's and
 *          6's.
 * https://oeis.org/A039072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: enumerate integers and keep those whose base‑8 representation has the
   same number of 4's and 6's.  a(1) = 0. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t len  = 0;   /* how many terms are stored */
    static size_t cap  = 0;   /* allocated capacity of cache */
    static seq_t next_candidate = 0;   /* candidate to test next */

    /* Allocate initial buffer (valid in both C and C++). */
    if (cache == NULL) {
        cap = 16;
#ifdef __cplusplus
        cache = new seq_t[cap];
#else
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
#endif
    }

    /* Generate terms until we have at least n of them. */
    while (len < n) {
        seq_t x = next_candidate;
        seq_t y = x;
        int   c4 = 0, c6 = 0;

        if (y != (seq_t)0) {
            while (y > (seq_t)0) {
                seq_t d = y % (seq_t)8;
                if (d == (seq_t)4) ++c4;
                else if (d == (seq_t)6) ++c6;
                y = y / (seq_t)8;
            }
        }

        if (c4 == c6) {                /* keep the number */
            if (len == cap) {          /* extend cache if full */
                size_t new_cap = cap * 2;
#ifdef __cplusplus
                seq_t *new_cache = new seq_t[new_cap];
                for (size_t i = 0; i < cap; ++i) new_cache[i] = cache[i];
                delete[] cache;
                cache = new_cache;
#else
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif
                cap = new_cap;
            }
            cache[len++] = x;
        }

        ++next_candidate;
    }

    return cache[n - 1];
}
