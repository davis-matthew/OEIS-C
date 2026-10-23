/*
 * A161706  a(n) = (-11*n^5 + 145*n^4 - 635*n^3 + 1115*n^2 - 494*n + 120)/120.
 * https://oeis.org/A161706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-11*n^5 + 145*n^4 - 635*n^3 + 1115*n^2 - 494*n +
 *            120)/120
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 121, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

static seq_t binom_small(idx_t n, int k)
{
    if (n < (idx_t)k) return 0;
    if (k == 0) return 1;
    seq_t res = 1;
    for (int i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(n - (idx_t)k + i);
        seq_t den = (seq_t)i;

        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        g = gcd(res, den);
        res /= g;
        den /= g;

        res *= num;
        if (den != 1) res /= den;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t res = 0;
    res += binom_small(n, 0); /* 1 */
    res += binom_small(n, 1);
    res += binom_small(n, 2);
    res -= (seq_t)2 * binom_small(n, 3);
    res += (seq_t)7 * binom_small(n, 4);
    res -= (seq_t)11 * binom_small(n, 5);
    return res;
}
