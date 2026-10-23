/*
 * A130612  Sum of the first 10^n squares.
 * https://oeis.org/A130612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*10^(3*n)+3*10^(2*n)+10^n)/6
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Compute 10^n, 10^(2n), and 10^(3n) */
    seq_t t   = pow10(n);
    seq_t t2  = pow10(2*n);
    seq_t t3  = pow10(3*n);

    /* numerator: 2*10^(3n) + 3*10^(2n) + 10^n */
    seq_t num = 2*t3 + 3*t2 + t;

    /* exact division by 6 gives the sum of squares */
    return num / 6;
}
