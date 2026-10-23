/*
 * A170965  Expansion of Product_{i=1..m} (1 + x^(4*i-1)) for m = 12.
 * https://oeis.org/A170965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 300, u16 n <= 300, u32 n <= 300, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the coefficient a(n) of x^n in
   ∏_{i=1}^{12} (1 + x^(4*i-1))   (exponents 3,7,11,...,47)   */
#include <stddef.h>

static const idx_t exps[12] = {3,7,11,15,19,23,27,31,35,39,43,47};
#define MAX_SUM 300

seq_t a(idx_t n) {
    static seq_t dp[MAX_SUM + 1] = {0};
    static int initialized = 0;

    if (!initialized) {
        dp[0] = (seq_t)1;
        for (idx_t k = 0; k < 12; ++k) {
            idx_t e = exps[k];
            for (idx_t s = MAX_SUM; s >= e; --s) {
                dp[s] = dp[s] + dp[s - e];
            }
        }
        initialized = 1;
    }

    if (n > MAX_SUM)
        return (seq_t)0;
    return dp[(size_t)n];
}
