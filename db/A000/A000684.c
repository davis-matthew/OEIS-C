/*
 * A000684  Number of colored labeled n-node graphs with 2 interchangeable
 *          colors.
 * https://oeis.org/A000684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Sum_{k = 0..n-1} binomial(n-1,k)*2^(k*(n-k))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 20,
 *            big any n
 * Helpers:   a684_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a684_pow2.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t binom = 1; /* binomial(n-1, k) */
    idx_t m = n - 1;
    idx_t k;
    for (k = 0; k < n; k++) {
        result += binom * a684_pow2(k * (n - k));
        if (k < m) {
            binom = binom * (seq_t)(m - k);
            binom = binom / (seq_t)(k + 1);
        }
    }
    return result;
}
