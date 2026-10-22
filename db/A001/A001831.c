/*
 * A001831  Number of labeled graded partially ordered sets with n elements of
 *          height at most 1.
 * https://oeis.org/A001831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Sum_{k=0..n} binomial(n, k)*(2^k-1)^(n-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 20,
 *            big any n
 * Helpers:   a1831_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a1831_pow.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t binom = 1;   /* C(n, k) */
    seq_t pow2k = 1;   /* 2^k */
    idx_t k;
    for (k = 0; k <= n; k++) {
        seq_t base = pow2k - 1;
        result += binom * a1831_pow(base, n - k);
        if (k < n) {
            binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);
            pow2k = pow2k * 2;
        }
    }
    return result;
}
