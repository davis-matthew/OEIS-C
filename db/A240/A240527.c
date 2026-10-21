/*
 * A240527  Indices of 7-almost prime triangular numbers.
 * https://oeis.org/A240527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 23, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A240527: indices m such that the m‑th triangular number
   T(m)=m(m+1)/2 is a 7‑almost prime.  The value a(n) is the n‑th such m
   (1‑based).  The function searches triangular numbers and memoises the
   results.  Only the function a() is defined; all types and helpers are
   supplied by the build environment. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;   /* stored indices m */
    static size_t  clen   = 0;      /* current number of entries */
    static size_t  ccap   = 0;      /* capacity of the cache */
    static idx_t   next_i = 1;      /* next triangular index to test */

    if (n == 0) return 0;   /* safety for 0‑based calls, not part of the offset */

    /* Fill the cache until at least n entries are available */
    while (clen < (size_t)n) {
        /* Scan upwards for the next triangular number with 7 prime factors */
        for (;;) {
            /* Compute T(next_i) = next_i*(next_i+1)/2 as seq_t to avoid overflow */
            seq_t tri = (seq_t)next_i * ((seq_t)next_i + 1) / 2;

            /* We need to factor tri.  bigomega expects an idx_t (uint64_t), so
               we rely on the fact that for the n values of interest the value
               of tri is always below 2^64.  The cast is safe under that
               assumption. */
            idx_t tri_n = (idx_t)tri;

            if (bigomega(tri_n) == 7) {
                /* Ensure enough space in the cache */
                if (clen == ccap) {
                    size_t new_cap = ccap ? ccap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                  new_cap * sizeof(seq_t));
                    cache = tmp;
                    ccap   = new_cap;
                }
                cache[clen++] = next_i;
                ++next_i;          /* advance for the next search */
                break;             /* exit the inner for to continue outer loop */
            }
            ++next_i;
        }
    }

    return cache[n - 1];
}
