/*
 * A007407  a(n) = denominator of Sum_{k=1..n} 1/k^2.
 * https://oeis.org/A007407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac, nice
 * Formula:   a(n) = denominator of Sum_{k=1..n} 1/k^2, built term by term:
 *            keep the fractional part r/den reduced, add 1/k^2, and remove
 *            common factors (these can only be primes dividing k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 46,
 *            big any n
 * Helpers:   reduce_by, sgcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/reduce_by.h"
#include "../utils/sgcd.h"

seq_t a(idx_t n) {
    seq_t r = 0, den = 1;   /* fractional part of the partial sum is r/den */
    for (idx_t k = 1; k <= n; k++) {
        seq_t k2 = (seq_t)k * (seq_t)k;
        if (k2 == 0) return den;  /* only possible on narrow fixed-width overflow */
        seq_t g = sgcd(k2, den % k2);
        seq_t u = den / g;
        seq_t f = k2 / g;
        seq_t nd = u * k2;
        seq_t t = r * f;
        /* r/den + 1/k^2 = (t + u)/nd, keep it below 1 without overflow */
        if (u >= nd - t)
            r = u - (nd - t);
        else
            r = t + u;
        den = nd;
        uint64_t x = (uint64_t)k;
        for (uint64_t p = 2; p * p <= x; p++) {
            if (x % p == 0) {
                while (x % p == 0) x /= p;
                reduce_by(&r, &den, p);
            }
        }
        if (x > 1) reduce_by(&r, &den, x);
    }
    return den;
}
