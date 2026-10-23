/*
 * A044122  Numbers n such that string 3,2 occurs in the base 6 representation
 *          of n but not of n-1.
 * https://oeis.org/A044122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   contains32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044122: n such that the base‑6 representation of n contains
   the substring "32" but the base‑6 representation of n‑1 does not.  This
   implementation searches for the n‑th such number by testing each
   integer in turn. */
#include <stdbool.h>
#include "../utils/contains32.h"

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t cur = 1;               /* start searching from 1 */
    while (1)                    /* loop until we locate the n‑th term */
    {
        if (contains32(cur))
        {
            if (!contains32(cur - (seq_t)1))
            {
                ++found;
                if (found == n)
                    return cur;
            }
        }
        ++cur;
    }
}
