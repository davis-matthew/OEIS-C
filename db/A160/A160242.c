/*
 * A160242  Triangle A(n,m) read by rows: a quarter of the Fourier coefficient
 *          [cos(m*t)] of the shifted Boubaker polynomial B_n(2*cos
 *          t)-2*cos(n*t).
 * https://oeis.org/A160242
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if n = 2 + k(k+1) for some k ≥ 0 (i.e., 4n−7 is an odd
 *            square); otherwise 2
 * Range:     u8 n <= 17, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Defined for n ≥ 2.  For smaller indices the value is irrelevant. */
    if (n < 2) return 2;

    seq_t val = (seq_t)4 * n - 7;           /* 4n−7           */
    seq_t s   = floor_sqrt(val);           /* floor(sqrt(val)) */

    if (s * s == val && (s & 1))           /* perfect square & odd */
        return 1;
    return 2;
}
