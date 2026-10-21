/*
 * A051382  Numbers k whose base 3 expansion matches (0|1)*(02)?(0|1)* (no
 *          more than one "02" allowed in midst of 0's and 1's).
 * https://oeis.org/A051382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 3602, u32 n <= 8856, u64 n <= 8856,
 *            u128 n <= 8856, big any n
 * Helpers:   valid_base3_pairs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (search) a(n) = the nth non‑negative integer k such that the base‑3
   representation of 2k contains only digits 0 or 2, with at most one
   pair of adjacent 1's.  The offset is 1, so a(1)=0. */
#include <stdbool.h>
#include "../utils/valid_base3_pairs.h"

/* OEIS sequence A051382: kth term in the list of integers whose
   base‑3 representation of 2k satisfies the 0/2 only with at most
   one adjacent pair of 1's. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t k = 0;

    while (1)
    {
        if (valid_base3_pairs(k))
        {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
}
