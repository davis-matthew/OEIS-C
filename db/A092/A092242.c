/*
 * A092242  Numbers that are congruent to {5, 7} (mod 12).
 * https://oeis.org/A092242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n - 3 - 2*(-1)^n
 * Range:     u8 n <= 42, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;               /* convert index to seq_t */
    seq_t val = 6 * nn - 3;            /* 6n - 3  */

    /* subtract (or add) 2 depending on the parity of n.
       (-1)^n is 1 for even n, -1 for odd n.            */
    if ((n & 1U) == 0)                    /* even n */
        val -= 2;
    else                                   /* odd n */
        val += 2;

    return val;
}
