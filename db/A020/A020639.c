/*
 * A020639  Lpf(n): least prime dividing n (when n > 1); a(1) = 1. Or,
 *          smallest prime factor of n, or smallest prime divisor of n.
 * https://oeis.org/A020639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, core
 * Formula:   a(n) = least prime factor of n, with a(1) = 1
 * Range:     u8 n <= 256, u16 n <= 9365, u32 n <= 9365, u64 n <= 9365,
 *            u128 n <= 9365, big any n
 * Helpers:   lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/lpf.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    return (seq_t)lpf(n);
}
