/*
 * A053253  Coefficients of the '3rd-order' mock theta function omega(q).
 * https://oeis.org/A053253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) is the coefficient of q^n in the mock theta function
 *            omega(q) = Σ_{k≥0} q^{2k(k+1)} / ((1-q)(1-q^3)…(1-q^{2k+1}))^2.
 *            It can be computed as a(n) = Σ_{k≥0, 2k(k+1)≤n}
 *            Σ_{t=0}^{n-2k(k+1)} q_k(t) * q_k(n-2k(k+1)-t) where q_k(m) is
 *            the number of partitions of m into odd parts not larger than
 *            2k+1...
 * Range:     u8 n <= 21, u16 n <= 63, u32 n <= 208, u64 n <= 730,
 *            u128 n <= 2688, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Assume seq_t and idx_t are supplied by the build. */
static seq_t *dp = NULL;      /* temporary array for partition counts */
static idx_t dp_size = 0;     /* current allocated size of dp */

seq_t a(idx_t n) {
    /* Allocate or enlarge the dp array if necessary. */
    if (dp_size < n + 1) {
        seq_t *newdp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
        if (!newdp) exit(1);          /* allocation failed */
        dp = newdp;
        dp_size = n + 1;
    }

    /* Initialize dp: dp[0] = 1, dp[>0] = 0. */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;

    seq_t result = 0;

    /* Find maximum k such that 2*k*(k+1) ≤ n. */
    idx_t kmax = 0;
    while (2 * kmax * (kmax + 1) <= n) ++kmax;
    --kmax;   /* last valid k */

    for (idx_t k = 0; k <= kmax; ++k) {
        idx_t j = 2 * k + 1;          /* new odd part to add */

        /* Update partition counts to include part j (unlimited copies). */
        for (idx_t s = j; s <= n; ++s) {
            dp[s] += dp[s - j];
        }

        idx_t m = n - 2 * k * (k + 1);   /* remaining exponent for this k */

        /* Convolution: sum_{t=0}^{m} dp[t] * dp[m-t] */
        seq_t sum = 0;
        for (idx_t t = 0; t <= m; ++t) {
            sum += dp[t] * dp[m - t];
        }
        result += sum;
    }

    return result;
}
