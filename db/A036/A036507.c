/*
 * A036507  Smallest square containing exactly n decimal digits '0'.
 * https://oeis.org/A036507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 0; for n >= 2, a(2*k) = 10^(2*k) = 10^n, a(2*k+1) =
 *            1024*10^(2*k) = 1024*10^(n-1)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 8, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    if ((n & (idx_t)1) == 0)          /* n even: a(n) = 10^n */
        return pow10(n);

    /* n odd: a(n) = 1024 * 10^(n-1) */
    seq_t t = pow10(n - 1);
    return t * (seq_t)1024;
}
