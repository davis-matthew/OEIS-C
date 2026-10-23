/*
 * A034967  Sum of digits of numbers between 0 and (10^n)-1.
 * https://oeis.org/A034967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 45*n*10^(n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t coeff = (seq_t)n * 45;
    seq_t p10   = pow10(n - 1);
    return coeff * p10;
}
