/*
 * A004313  a(n) = binomial coefficient C(2n, n-7).
 * https://oeis.org/A004313
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(2n, n-7), computed multiplicatively as C(m,i) =
 *            C(m,i-1) * (m-k+i) / i with m = 2n, k = n-7, cancelling gcds
 *            first so no intermediate value exceeds the next partial binomial
 *            C(n+7+i, i)
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 19, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_seq.h"

seq_t a(idx_t n) {
    idx_t k = n - 7;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        seq_t num = (seq_t)(n + 7) + (seq_t)i;
        seq_t den = (seq_t)i;
        seq_t g = gcd_seq(r, den);
        r /= g;
        den /= g;
        num /= den;
        r *= num;
    }
    return r;
}
