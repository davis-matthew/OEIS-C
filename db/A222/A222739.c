/*
 * A222739  Partial sums of the first 10^n terms in A181482.
 * https://oeis.org/A222739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^n + 2)*(10^(2*n) + 10^n + 4)/18
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Compute 10^n and 10^(2n) */
    seq_t ten_n  = pow10(n);
    seq_t ten_2n = pow10(n + n);

    /* Compute numerator components */
    seq_t num1 = ten_n + (seq_t)2;
    seq_t num2 = ten_2n + ten_n + (seq_t)4;

    /* Final value: numerator divided by 18 */
    return (num1 * num2) / (seq_t)18;
}
