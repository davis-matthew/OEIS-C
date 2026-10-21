/*
 * A084213  Binomial transform of A081250.
 * https://oeis.org/A084213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (5*4^n - 2^(n+1) + 0^n)/4 This can be simplified to a(0)
 *            = 1 and, for n >= 1, a(n) = 5*4^(n-1) - 2^(n-1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the zero index explicitly */
    if (n == 0u)
        return (seq_t)1;

    /* 4^(n-1) = 2^(2*(n-1)) */
    seq_t pow4 = pow2(2 * (n - 1u));

    /* 2^(n-1) */
    seq_t pow2v = pow2(n - 1u);

    /* a(n) = 5*4^(n-1) - 2^(n-1) */
    return (seq_t)5 * pow4 - pow2v;
}
