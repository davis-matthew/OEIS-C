/*
 * A173732  a(n) = (A016957(n)/2^A007814(A016957(n)) - 1)/2, with A016957(n) =
 *          6*n+4 and A007814(n) the 2-adic valuation of n.
 * https://oeis.org/A173732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ( (6*n+4) / 2^v2(6*n+4) - 1 ) / 2 , where v2(m) is the
 *            number of trailing zeros of m in binary
 * Range:     u8 n <= 41, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* m = 6*n + 4 */
    seq_t m = (seq_t)n * (seq_t)6 + (seq_t)4;

    /* divide m by 2 repeatedly while it is even */
    while ((m & (seq_t)1) == (seq_t)0) {
        m >>= 1;               /* shift by one bit only */
    }

    /* now m is odd; the required value is (m - 1)/2 */
    return (m - (seq_t)1) >> 1;
}
