/*
 * A100381  a(n) = 2^n*binomial(n,2).
 * https://oeis.org/A100381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * binomial(n,2) = n * (n-1) * 2^(n-1)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 53, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* optional, types are provided by build */

seq_t a(idx_t n)
{
    /* Base cases: a(0) = a(1) = 0 */
    if (n < 2)
        return 0;

    /* Convert index to seq_t for arithmetic */
    seq_t nn = (seq_t)n;

    /* Compute 2^(n-1) using left shift */
    seq_t pow2 = (seq_t)1;
    pow2 <<= (n - 1);

    /* a(n) = n * (n-1) * 2^(n-1) */
    return nn * (nn - 1) * pow2;
}
