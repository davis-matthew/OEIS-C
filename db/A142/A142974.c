/*
 * A142974  A007318 * [1, 1, -1, 1, 1, 1, ...].
 * https://oeis.org/A142974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n - 2 - 2*T(n) + 2^(n-1) where T(n) = n*(n+1)/2 and the
 *            sequence is defined for n ≥ 1
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    /* a(0) is undefined – return 0 for safety */
    if (n == 0) return 0;

    seq_t sn  = (seq_t)n;
    seq_t tri = sn * (sn + 1) / 2;          /* T(n) */
    seq_t val = 4 * sn - 2 - 2 * tri + pow2(n - 1);
    return val;
}
