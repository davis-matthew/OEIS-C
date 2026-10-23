/*
 * A034585  Dimension of an irreducible Z_2 graded H-module for Clifford
 *          algebra Cl_n.
 * https://oeis.org/A034585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16 * a(n-8) for n > 8, with a(1)=2, a(2)=2, a(3)=2,
 *            a(4)=2, a(5)=4, a(6)=8, a(7)=16, a(8)=16
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    static const seq_t base[8] = {
        (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)2,
        (seq_t)4, (seq_t)8, (seq_t)16, (seq_t)16
    };

    if (n <= 8)
        return base[n - 1];

    /* number of full blocks of 8 preceding n */
    idx_t e = (n - 1) / 8;
    /* each block multiplies by 16 = 2^4, so exponent grows by 4 per block */
    idx_t powexp = 4 * e;
    seq_t mult = pow2(powexp);          /* 2^(4*e) */
    return base[(n - 1) % 8] * mult;     /* base * 2^(4*e) */
}
