/*
 * A094085  Denominator of (3*2^(n-1) - 1)*integral_{x=0 to 1/(4^n)}1-sqrt x
 *          dx.
 * https://oeis.org/A094085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 for n = 1 * a(n) = 12 * 8^(n-1) for n > 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the utility pow2(exp) to compute 2^exp,
 * where exp = 3*(n-1), since 8^(n-1) = 2^(3*(n-1)). */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)3;
    idx_t exp = 3 * (n - 1);          /* exponent for 2^exp = 8^(n-1) */
    seq_t val = pow2(exp);            /* compute 2^exp */
    return val * (seq_t)12;           /* multiply by 12 to get a(n) */
}
