/*
 * A039326  Numbers whose base-7 representation has the same nonzero number of
 *          4's and 5's.
 * https://oeis.org/A039326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: scan integers, keep those whose base‑7 representation has
   a nonzero, equal number of 4’s and 5’s. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t  *seq = NULL;      /* cache of sequence values   */
    static size_t  cap  = 0;        /* allocated capacity        */
    static idx_t   cur  = 0;        /* last integer tested       */
    static idx_t   sz   = 0;        /* number of values stored   */

    /* extend cache until it contains a(n) */
    while ((idx_t)sz < n) {
        ++cur;                       /* candidate integer         */

        /* count 4's and 5's in base‑7 representation */
        idx_t  x  = cur;
        unsigned int c4 = 0, c5 = 0;
        do {
            int d = x % 7;
            if (d == 4) ++c4;
            else if (d == 5) ++c5;
            x /= 7;
        } while (x);

        if (c4 == c5 && c4 != 0) {
            if (sz >= cap) {          /* grow cache if needed */
                cap = cap? cap*2 : 16;
                seq = mem_resize(seq_t, seq, cap);
            }
            seq[sz++] = (seq_t)cur;   /* store qualifying value  */
        }
    }
    return seq[n-1];
}
