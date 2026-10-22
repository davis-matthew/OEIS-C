/*
 * A004360  Binomial coefficient C(6n,n-5).
 * https://oeis.org/A004360
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(6*n, n-5), computed multiplicatively as C(m,k) =
 *            prod_{i=1..k} (m-k+i)/i with gcd reduction so every intermediate
 *            value stays <= the result
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 36,
 *            big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_seq.h"

seq_t a(idx_t n) {
    if (n < 5) return 0;
    idx_t m = 6 * n;
    idx_t k = n - 5;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        seq_t num = (seq_t)(m - k + i);
        seq_t den = (seq_t)i;
        seq_t g = gcd_seq(r, den);
        r /= g;
        den /= g;
        num /= den;
        r *= num;
    }
    return r;
}
