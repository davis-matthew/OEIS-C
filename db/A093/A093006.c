/*
 * A093006  Referring to the triangle in A093005, sequence contains the least
 *          term with maximal number of divisors.
 * https://oeis.org/A093006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m in the range n(n-1)/2 < m <= n(n+1)/2 maximizing
 *            d(m), the number of divisors (found by search; row n of the
 *            triangle in A093005 is that range)
 * Range:     u8 n <= 22, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A093006_ndiv(idx_t m)
{
    idx_t c = 0, d;
    for (d = 1; d * d <= m; d++) {
        if (m % d == 0) {
            c++;
            if (d * d != m) c++;
        }
    }
    return c;
}

seq_t a(idx_t n)
{
    idx_t lo = n * (n - 1) / 2 + 1;
    idx_t hi = n * (n + 1) / 2;
    idx_t best = lo, bestc = 0, m;
    for (m = lo; m <= hi; m++) {
        idx_t c = A093006_ndiv(m);
        if (c > bestc) {
            bestc = c;
            best = m;
        }
    }
    return (seq_t)best;
}
