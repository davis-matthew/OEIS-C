/*
 * A093417  Row sums of A093415.
 * https://oeis.org/A093417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} (k+1)/gcd(2n+2, k+1)
 * Range:     u8 n <= 27, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A093417_gcd(idx_t x, idx_t y)
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
    idx_t m = 2 * n + 2;
    for (idx_t k = 1; k <= n; k++) {
        idx_t d = k + 1;
        sum += (seq_t)(d / A093417_gcd(m, d));
    }
    return sum;
}
