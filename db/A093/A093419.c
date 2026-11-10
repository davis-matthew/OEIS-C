/*
 * A093419  Denominators of row sums in triangle described in A093412.
 * https://oeis.org/A093419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of sum_{k=1..n} (2n+1-k)/(k+1) =
 *            (2n+2)*(H(n+1)-1) - n, where H is the harmonic number; computed
 *            as den(H(n+1)) / gcd(2n+2, den(H(n+1)))
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A093419_gcd(seq_t x, seq_t y)
{
    while (y != (seq_t)0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    seq_t p = 1, q = 1;  /* H(1) = 1/1 */
    for (idx_t j = 2; j <= n + 1; j++) {
        seq_t jj = (seq_t)j;
        seq_t g = A093419_gcd(q, jj);
        seq_t L = q / g * jj;           /* lcm(q, j) */
        p = p * (L / q) + L / jj;
        q = L;
        seq_t h = A093419_gcd(p, q);
        if (h != (seq_t)1) {
            p /= h;
            q /= h;
        }
    }
    seq_t m = (seq_t)2 * (seq_t)n + 2;
    seq_t g = A093419_gcd(m, q);
    return q / g;
}
