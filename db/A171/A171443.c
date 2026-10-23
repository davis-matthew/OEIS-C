/*
 * A171443  Expansion of g.f. (1+x)^8/(1-x).
 * https://oeis.org/A171443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(9, n-2*k)
 * Range:     u8 n <= 3, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* For n ≥ 8, all terms with r > 9 vanish and the sum equals 256. */
    if (n >= 8) {
        return (seq_t)256;
    }

    seq_t sum = 0;
    for (idx_t k = 0; 2 * k <= n; ++k) {
        idx_t r = n - 2 * k;
        sum += binomial((idx_t)9, r);
    }
    return sum;
}
