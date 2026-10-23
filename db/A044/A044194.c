/*
 * A044194  Numbers n such that string 0,7 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers n such that the substring "07" occurs in the base‑8 *
 *            representation of n but not in that of n‑1. * Computation:
 *            brute‑force scan incrementally until the n‑th such * number is
 *            found
 * Range:     u8 n <= 3, u16 n <= 1142, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_sub07
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/has_sub07.h"

seq_t a(idx_t n)
{
    idx_t count = 0;   /* number of terms found so far */
    idx_t x = 0;       /* current integer to test */

    for (;;)
    {
        ++x;
        if (has_sub07(x) && !has_sub07(x - 1))
        {
            ++count;
            if (count == n)
                return (seq_t)x;
        }
    }
}
