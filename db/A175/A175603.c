/*
 * A175603  a(n) = 8*(10^n-5).
 * https://oeis.org/A175603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 8*(10^n-5)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Compute 10^n using the utility function. */
    seq_t p10 = pow10(n);

    /* a(n) = 8*(10^n-5) = 8*(p10-5) */
    seq_t res = p10 - (seq_t)5;
    res *= (seq_t)8;
    return res;
}
