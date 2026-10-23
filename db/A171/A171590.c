/*
 * A171590  a(n) = 1+4^(n+1)-4*(-2)^n.
 * https://oeis.org/A171590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + 4^(n+1) - 4*(-2)^n
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    seq_t n_seq = (seq_t)n;

    /* Compute 4^(n+1) */
    seq_t pow4 = big_pow((seq_t)4, n_seq + 1);

    /* Compute 2^n and 4*2^n */
    seq_t pow2n      = big_pow((seq_t)2, n_seq);
    seq_t fourPow2n  = 4 * pow2n;

    /* For even n: (-2)^n = 2^n, so subtract 4*2^n.
       For odd n:  (-2)^n = -2^n, so subtract (-4*2^n) which is +4*2^n. */
    if ((n & 1) == 0) {
        return 1 + pow4 - fourPow2n;
    } else {
        return 1 + pow4 + fourPow2n;
    }
}
