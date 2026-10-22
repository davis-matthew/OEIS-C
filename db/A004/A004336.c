/*
 * A004336  Binomial coefficient C(4n,n-6).
 * https://oeis.org/A004336
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(4*n, n-6)
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 24, u128 n <= 43,
 *            big any n
 * Helpers:   a004336_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004336_gcd.h"

seq_t a(idx_t n) {
    idx_t k = n - 6;
    idx_t m = 4 * n;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        /* r = C(m-k+i, i) computed as C(m-k+i-1, i-1) * (m-k+i) / i, exactly */
        seq_t num = (seq_t)(m - k + i);
        seq_t den = (seq_t)i;
        seq_t g = a004336_gcd(r, den);
        r /= g;
        den /= g;
        num /= den;
        r *= num;
    }
    return r;
}
