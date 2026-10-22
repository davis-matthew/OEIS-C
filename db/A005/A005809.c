/*
 * A005809  a(n) = binomial(3n,n).
 * https://oeis.org/A005809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = binomial(3n,n), computed multiplicatively: C(2n+i,i) =
 *            C(2n+i-1,i-1)*(2n+i)/i for i = 1..n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   a005809_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a005809_gcd.h"

seq_t a(idx_t n) {
    seq_t r = 1;
    seq_t two_n = (seq_t)n * 2;
    idx_t i;
    for (i = 1; i <= n; i++) {
        seq_t num = two_n + (seq_t)i;
        seq_t den = (seq_t)i;
        seq_t g = a005809_gcd(r, den);
        r /= g;
        den /= g;
        /* den now divides num exactly, since r*num/i is an integer */
        r *= num / den;
    }
    return r;
}
