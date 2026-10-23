/*
 * A043846  Numbers n such that number of runs in the base 4 representation of
 *          n is congruent to 3 mod 7.
 * https://oeis.org/A043846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   count_runs_base4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth integer whose number of runs in its base‑4
 * representation is congruent to 3 modulo 7.  The sequence is generated
 * by successive testing of integers. */
#include <stdlib.h>
#include "../utils/count_runs_base4.h"

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* cached terms */
    static size_t  cap    = 0;      /* allocated capacity */
    static size_t  len    = 0;      /* number of terms cached */
    static seq_t   last   = 0;      /* last number checked */

    if (n == 0)                /* offset is 1 – returning 0 for n==0 is harmless */
        return 0;

    /* Ensure the cache holds at least n terms */
    if (len < (size_t)n) {
        /* grow cache if necessary */
        if (cap < (size_t)n) {
            size_t newcap = cap ? cap * 2 : 32;
            if (newcap < (size_t)n)
                newcap = (size_t)n;
            /* cast needed for C++ compatibility when seq_t is a class */
            seq_t *ptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!ptr) return 0;          /* allocation failure – silent */
            cache = ptr;
            cap   = newcap;
        }

        /* extend cache by checking successive integers */
        seq_t cand = last + 1;
        while (len < (size_t)n) {
            if (count_runs_base4(cand) % 7 == 3)
                cache[len++] = cand;
            cand++;
        }
        last = cand - 1;
    }

    return cache[n-1];
}
