/*
 * A004372  Binomial coefficient C(7n,n-4).
 * https://oeis.org/A004372
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(7n, n-4) = Product_{i=1..n-4} (6n+4+i)/i
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 22,
 *            big any n
 * Helpers:   a004372_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004372_gcd.h"

seq_t a(idx_t n) {
    idx_t k = n - 4;
    seq_t base = (seq_t)6 * (seq_t)n + 4;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        seq_t num = base + (seq_t)i;
        seq_t den = (seq_t)i;
        seq_t g = a004372_gcd(r, den);
        r = r / g;
        den = den / g;
        r = r * (num / den);
    }
    return r;
}
