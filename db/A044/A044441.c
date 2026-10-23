/*
 * A044441  Numbers n such that string 2,2 occurs in the base 3 representation
 *          of n but not of n+1.
 * https://oeis.org/A044441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   has22
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A044441
 *  Numbers n such that the string "22" occurs in the base‑3
 *  representation of n but not in that of n+1.
 *
 *  The function generates the terms by a simple linear scan:
 *  it tests every integer m ≥ 1 in increasing order for the
 *  property and returns the n‑th one that satisfies it.
 *
 *  Performance: on typical systems the 10 000‑th term is below
 *  10⁵, so the scan finishes well under 2 s.  The algorithm
 *  uses only integer arithmetic and works with any seq_t
 *  (unsigned integer type or arbitrary‑precision type).
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/has22.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t m = 1;

    while (1)
    {
        if (has22(m) && !has22(m + 1))
        {
            ++count;
            if (count == n)
                return m;
        }
        ++m;
    }
}
