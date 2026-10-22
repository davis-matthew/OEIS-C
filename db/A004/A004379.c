/*
 * A004379  Binomial coefficient C(7n,n-11).
 * https://oeis.org/A004379
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(7*n, n-11)
 * Range:     u8 n <= 12, u16 n <= 13, u32 n <= 17, u64 n <= 24, u128 n <= 28,
 *            big any n
 * Helpers:   a004379_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004379_gcd.h"

seq_t a(idx_t n) {
    idx_t k = n - 11;
    idx_t m = 7 * n;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        /* r = r * (m - k + i) / i, reduced so no intermediate exceeds the result */
        seq_t num = (seq_t)(m - k + i);
        seq_t den = (seq_t)i;
        seq_t g = a004379_gcd(r, den);
        r /= g;
        den /= g;
        num /= den;
        r *= num;
    }
    return r;
}
