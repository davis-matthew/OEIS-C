/*
 * A001791  a(n) = binomial coefficient C(2n, n-1).
 * https://oeis.org/A001791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = binomial(2n, n-1), computed as C(n+1+k, k) for k =
 *            1..n-1: r_k = r_{k-1} * (n+1+k) / k, r_0 = 1, with gcd reduction
 *            to avoid overflow
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   a001791_sgcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a001791_sgcd.h"

seq_t a(idx_t n) {
    seq_t r = 1;
    idx_t i;
    if (n == 0)
        return 0;
    for (i = 1; i <= n - 1; i++) {
        seq_t num = (seq_t)n + 1 + (seq_t)i;
        seq_t den = (seq_t)i;
        seq_t g = a001791_sgcd(r, den);
        r = r / g;
        den = den / g;
        num = num / den;
        r = r * num;
    }
    return r;
}
