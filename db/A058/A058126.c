/*
 * A058126  a(n) = n^n - n^2 with 0^0=1.
 * https://oeis.org/A058126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^n - n^2 (with 0^0 = 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* compute n^n (0^0 == 1) */
    seq_t pow_n = big_pow(nn, nn);
    /* compute n^2 */
    seq_t sq = nn * nn;
    return pow_n - sq;
}
