/*
 * A188897  a(n) = lcm(n, n+1, n+2, n+3, n+4, n+5, n+6, n+7)/840.
 * https://oeis.org/A188897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(n, n+1, n+2, n+3, n+4, n+5, n+6, n+7)/840
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 26, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t l = 1;
    for (idx_t i = 0; i < 8; ++i) {
        seq_t x = (seq_t)(n + i);
        seq_t g = gcd(l, x);
        l = (l / g) * x;
    }
    return l / (seq_t)840;
}
