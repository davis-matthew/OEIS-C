/*
 * A001700  a(n) = binomial(2*n+1, n+1): number of ways to put n+1
 *          indistinguishable balls into n+1 distinguishable boxes = number of
 *          (n+1)-st degree monomials in n+1 variables = number of monotone
 *          maps from 1..n+1 to 1..n+1.
 * https://oeis.org/A001700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice, core, changed
 * Formula:   a(n) = binomial(2*n+1, n), built up as C(n+1+i, i) for i = 1..n
 *            with gcd reduction so no intermediate exceeds the result
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   a001700_gcd_s
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a001700_gcd_s.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    idx_t i;
    for (i = 1; i <= n; i++) {
        seq_t m = (seq_t)n + 1 + (seq_t)i;
        seq_t d = (seq_t)i;
        seq_t g = a001700_gcd_s(result, d);
        result = (result / g) * (m / (d / g));
    }
    return result;
}
