/*
 * A128797  a(n) = (n^2-n)*3^n.
 * https://oeis.org/A128797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*3^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 15, u64 n <= 33, u128 n <= 72,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* For n = 0 or 1 the formula gives 0, avoid underflow of unsigned */
    if (n < 2)
        return 0;

    /* Compute n*(n-1) in seq_t */
    seq_t nn = (seq_t)n;
    seq_t mult = nn * (nn - 1);

    /* Compute 3^n using the provided big integer power routine */
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);

    /* Final result */
    return mult * pow3;
}
