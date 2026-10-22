/*
 * A004351  Binomial coefficient C(5*n,n-9).
 * https://oeis.org/A004351
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(5*n, n-9), computed multiplicatively as C(m,k) =
 *            prod_{i=1..k} (m-k+i)/i with gcd reduction, m = 5n, k = n-9
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 16, u64 n <= 24, u128 n <= 27,
 *            big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_seq.h"

seq_t a(idx_t n) {
    idx_t m = 5 * n;
    idx_t k = n - 9;
    idx_t i;
    seq_t r = 1;
    if (k > m - k) k = m - k;
    for (i = 1; i <= k; i++) {
        seq_t num = (seq_t)(m - k + i);
        seq_t den = (seq_t)i;
        seq_t g = gcd_seq(r, den);
        r = (r / g) * (num / (den / g));
    }
    return r;
}
