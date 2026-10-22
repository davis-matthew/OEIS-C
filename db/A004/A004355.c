/*
 * A004355  Binomial coefficient C(6n,n).
 * https://oeis.org/A004355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(6*n,n) = Product_{i=1..n} (5*n+i)/i, computed
 *            multiplicatively with gcd reduction
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   a004355_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004355_gcd.h"

seq_t a(idx_t n) {
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= n; i++) {
        seq_t num = (seq_t)5 * (seq_t)n + (seq_t)i;
        seq_t den = (seq_t)i;
        seq_t g = a004355_gcd(r, den);
        r /= g;
        den /= g;
        /* r*num/i is an integer and gcd(r, den) = 1, so den divides num */
        r *= num / den;
    }
    return r;
}
