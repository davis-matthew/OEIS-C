/*
 * A070198  Smallest nonnegative number m such that m == i (mod i+1) for all 1
 *          <= i <= n.
 * https://oeis.org/A070198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = lcm(1,2,...,n+1) - 1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 45, u128 n <= 87,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t l = 1;
    idx_t limit = n + 1;
    for (idx_t i = 2; i <= limit; ++i) {
        seq_t x = (seq_t)i;
        seq_t g = gcd(l, x);
        l = l / g * x;
    }
    return l - 1;
}
