/*
 * A001449  Binomial coefficients binomial(5n,n).
 * https://oeis.org/A001449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(5*n, n), computed multiplicatively as C(5n,i) =
 *            C(5n,i-1) * (5n-i+1) / i for i = 1..n, dividing by gcd first so
 *            no intermediate exceeds the final value
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   a001449_gcd_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a001449_gcd_u.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    idx_t i;
    for (i = 1; i <= n; i++) {
        idx_t m = 5 * n - i + 1;
        uint64_t r = (uint64_t)(result % (seq_t)i);
        uint64_t g = a001449_gcd_u(r, (uint64_t)i);
        uint64_t d = (uint64_t)i / g;
        result = (result / (seq_t)g) * (seq_t)(m / d);
    }
    return result;
}
