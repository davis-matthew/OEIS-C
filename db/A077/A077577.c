/*
 * A077577  a(n) = floor(A062273(n)/n).
 * https://oeis.org/A077577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = floor(A062273(n)/n), A062273(n) = Sum_{i=1..n}
 *            ((n*(n-1)/2 + i) mod 10) * 10^(n-i); computed by long division
 *            digit by digit
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t x = n, y = n - 1;
    if (x % 2 == 0)
        x /= 2;
    else
        y /= 2;
    unsigned base = (unsigned)(((x % 10) * (y % 10)) % 10);

    seq_t q = 0;
    seq_t rem = 0;
    seq_t nn = (seq_t)n;
    for (idx_t i = 1; i <= n; ++i) {
        unsigned digit = (base + (unsigned)(i % 10)) % 10U;
        seq_t cur = rem * 10 + (seq_t)digit;
        q = q * 10 + cur / nn;
        rem = cur % nn;
    }
    return q;
}
