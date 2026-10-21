/*
 * A083345  Numerator of r(n) = Sum(e/p: n=Product(p^e)); a(n) = n' /
 *          gcd(n,n'), where n' is the arithmetic derivative of n.
 * https://oeis.org/A083345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = n' / gcd(n,n'), where n' = n * Σ e/p over prime powers
 *            p^e||n and gcd is the integer greatest common divisor
 * Range:     u8 n <= 267, u16 n <= 9612, u32 n <= 9612, u64 n <= 9612,
 *            u128 n <= 9612, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n == 1) return 0;   /* a(1) = 0 */

    uint64_t temp = (uint64_t)n;            /* remaining part to factor */
    seq_t n_seq = (seq_t)n;                 /* n in seq_t form */
    seq_t derivative = 0;                   /* n' accumulator */

    while (temp > 1) {
        /* find smallest prime factor p of temp by trial division */
        uint64_t p = 2;
        for (; p * p <= temp; ++p) {
            if (temp % p == 0) break;
        }
        if (p * p > temp) {                  /* temp itself is prime */
            p = temp;
        }

        /* count exponent e of this prime factor */
        uint64_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }

        /* term = e * (n/p) */
        seq_t term = (seq_t)e * (n_seq / (seq_t)p);
        derivative += term;
    }

    seq_t g = gcd(n_seq, derivative);
    return derivative / g;
}
