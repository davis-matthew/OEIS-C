/*
 * A004343  Binomial coefficient C(5n,n-1).
 * https://oeis.org/A004343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(5*n, n-1), computed multiplicatively as C(m,k) =
 *            Prod_{i=1..k} (m-k+i)/i with gcd reduction so no intermediate
 *            exceeds the result
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_seq.h"

seq_t a(idx_t n) {
    idx_t m = 5 * n;
    idx_t k = n - 1;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        seq_t num = (seq_t)(m - k + i);
        seq_t den = (seq_t)i;
        seq_t g = gcd_seq(r, den);
        r = (r / g) * (num / (den / g));
    }
    return r;
}
