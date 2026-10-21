/*
 * A081038  3rd binomial transform of (1,2,0,0,0,0,0,0,...).
 * https://oeis.org/A081038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n + 3) * 3^(n-1), with a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;  /* a(0)=1 */

    /* 3^(n-1) */
    seq_t pow3 = big_pow((seq_t)3, (seq_t)(n - 1));

    /* multiplier (2*n + 3) */
    seq_t mult = (seq_t)n * (seq_t)2 + (seq_t)3;

    return pow3 * mult;
}
