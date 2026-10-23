/*
 * A044622  Numbers n such that string 6,6 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   has66_oct
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence computed by linear search:
   a(n) = the n‑th integer whose base‑8 representation contains
   the substring "66" but the representation of n+1 does not.
   (Starting with a(1) = 54.) */

#include <stdbool.h>
#include "../utils/has66_oct.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;          /* offset is 1, just for safety */

    seq_t current = (seq_t)0;
    idx_t count = (idx_t)0;

    while (count < n)
    {
        ++current;
        if (has66_oct(current) && !has66_oct(current + (seq_t)1))
            ++count;
    }
    return current;
}
