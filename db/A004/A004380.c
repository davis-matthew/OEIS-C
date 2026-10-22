/*
 * A004380  Binomial coefficient C(7n,n-12).
 * https://oeis.org/A004380
 *
 * Offset:    12    (a(12) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(7*n, n-12), computed multiplicatively as
 *            C(6n+12+i, i) = C(6n+12+i-1, i-1) * (6n+12+i) / i for i =
 *            1..n-12
 * Range:     u8 n <= 13, u16 n <= 14, u32 n <= 17, u64 n <= 25, u128 n <= 29,
 *            big any n
 * Helpers:   a004380_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004380_gcd.h"

seq_t a(idx_t n) {
    idx_t k = n - 12;
    idx_t base = 6 * n + 12; /* 7n - k */
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        seq_t num = (seq_t)base + (seq_t)i;
        seq_t den = (seq_t)i;
        seq_t g = a004380_gcd(r, den);
        r /= g;
        den /= g;
        /* den now divides num exactly, since r*num is divisible by i */
        r *= num / den;
    }
    return r;
}
