/*
 * A078546  LCM of n and its nonzero decimal digits.
 * https://oeis.org/A078546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = lcm(n, all nonzero decimal digits of n)
 * Range:     u8 n <= 26, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t l = (seq_t)n;
    idx_t x = n;

    while (x > 0) {
        int d = x % 10;
        if (d != 0) {
            seq_t g = gcd(l, (seq_t)d);
            l = l / g * (seq_t)d;
        }
        x /= 10;
    }
    return l;
}
