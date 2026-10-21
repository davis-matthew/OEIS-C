/*
 * A240440  Number of ways to place 3 points on a triangular grid of side n so
 *          that they are not vertices of an equilateral triangle of any
 *          orientation.
 * https://oeis.org/A240440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+3)*(n+2)*(n+1)*n*(n-1)*(n-2)/48
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 76, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 2)
        return 0;
    seq_t nn = (seq_t)n;
    seq_t factors[6] = {
        nn - 2,
        nn - 1,
        nn,
        nn + 1,
        nn + 2,
        nn + 3
    };
    seq_t res = 1;
    seq_t div = 48;
    for (int i = 0; i < 6; ++i) {
        seq_t g = gcd(factors[i], div);
        div /= g;
        res *= factors[i] / g;
    }
    return res;
}
