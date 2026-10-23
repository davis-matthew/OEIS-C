/*
 * A162629  G.f. is the polynomial (1-x^3) * (1-x^6) * (1-x^9) * (1-x^12) *
 *          (1-x^15) * (1-x^18) * (1-x^21) * (1-x^24) * (1-x^27) * (1-x^30) *
 *          (1-x^33) * (1-x^36) / (1-x)^12.
 * https://oeis.org/A162629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1}^{12} (1 + x + ... +
 *            x^{3k-1}) The generating function given by (∏_{k=1}^{12}
 *            (1−x^{3k})) / (1−x)^{12} simplifies to the product of the 12
 *            polynomials above. The sequence has non‑zero terms only for 0 ≤
 *            n ≤ 222. The values are computed once by dynamic programmi...
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 34, u64 n <= 222, u128 n <= 222,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static seq_t _a_table[223];
static bool _a_computed = false;

static void _compute_a_table(void) {
    const idx_t MAX_DEG = 222;
    seq_t dp[223] = {0}, ndp[223];
    dp[0] = 1;
    for (idx_t k = 1; k <= 12; ++k) {
        idx_t limit = 3 * k;            /* number of terms in each factor */
        /* zero ndp */
        for (idx_t i = 0; i <= MAX_DEG; ++i) ndp[i] = 0;
        /* convolution with a block of ones of length 'limit' */
        for (idx_t j = 0; j <= MAX_DEG; ++j) {
            seq_t sum = 0;
            for (idx_t t = 0; t < limit; ++t) {
                if (j >= t) sum += dp[j - t];
            }
            ndp[j] = sum;
        }
        /* copy ndp back to dp for next iteration */
        for (idx_t i = 0; i <= MAX_DEG; ++i) dp[i] = ndp[i];
    }
    for (idx_t i = 0; i <= MAX_DEG; ++i) _a_table[i] = dp[i];
    _a_computed = true;
}

seq_t a(idx_t n) {
    if (!_a_computed) _compute_a_table();
    if (n <= 222) return _a_table[n];
    return 0;
}
