/*
 * A077666  Sum of terms of n-th row of A077664.
 * https://oeis.org/A077666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the n smallest integers k > n with gcd(k,n) = 1
 *            (row n of A077664); computed by direct scan
 * Range:     u8 n <= 11, u16 n <= 161, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 966 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A077666_gcd(idx_t x, idx_t y)
{
    while (y) {
        idx_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t count = 0;
    idx_t cur = n + 1;
    if (n == 0) return 0;
    for (; count < n; ++cur) {
        if (A077666_gcd(n, cur) == 1) {
            sum += (seq_t)cur;
            ++count;
        }
    }
    return sum;
}
