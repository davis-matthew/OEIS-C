/*
 * A143278  Convolution of A006352 and A010815.
 * https://oeis.org/A143278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (n==0?1:-24*σ(n)) + * Σ_{k>0} (-1)^k * (
 *            (n-p1==0?1:-24σ(n-p1)) + (n-p2==0?1:-24σ(n-p2)) ), * where p1 =
 *            k(3k-1)/2, p2 = k(3k+1)/2 are the generalized pentagonal numbers
 *            * and σ(m) is the sum of divisors of m
 * Range:     i8 n <= 6, i16 n <= 89, i32 n <= 89, i64 n <= 89, i128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t sigma_value(idx_t m)
{
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            sum += (seq_t)d;
            idx_t e = m / d;
            if (e != d) sum += (seq_t)e;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t result = (n == 0) ? (seq_t)1 : -(seq_t)24 * sigma_value(n);

    for (idx_t k = 1; ; ++k) {
        idx_t p1 = k * (3 * k - 1) / 2;
        idx_t p2 = k * (3 * k + 1) / 2;
        if (p1 > n && p2 > n)
            break;

        seq_t sign = ((k & 1u) ? (seq_t)-1 : (seq_t)1);

        if (p1 <= n) {
            idx_t m = n - p1;
            seq_t term = (m == 0) ? (seq_t)1 : -(seq_t)24 * sigma_value(m);
            result += sign * term;
        }
        if (p2 <= n) {
            idx_t m = n - p2;
            seq_t term = (m == 0) ? (seq_t)1 : -(seq_t)24 * sigma_value(m);
            result += sign * term;
        }
    }
    return result;
}
