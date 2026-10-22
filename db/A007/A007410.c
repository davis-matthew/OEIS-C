/*
 * A007410  Numerator of Sum_{k=1..n} k^(-4).
 * https://oeis.org/A007410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator(Sum_{k=1..n} 1/k^4), summed term by term with
 *            reduction at each step
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   a007410_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a007410_gcd.h"

seq_t a(idx_t n) {
    seq_t p = 0, q = 1;

    for (idx_t k = 1; k <= n; k++) {
        seq_t ks = (seq_t)k;
        seq_t k4 = ks * ks * ks * ks;

        /* p/q + 1/k4 over the common denominator lcm(q, k4) */
        seq_t g1 = a007410_gcd(q, k4);
        seq_t m = k4 / g1;
        seq_t np = p * m + q / g1;
        seq_t nq = q * m;

        seq_t g = a007410_gcd(np, nq);
        p = np / g;
        q = nq / g;
    }

    return p;
}
