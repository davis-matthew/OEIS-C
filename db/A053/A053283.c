/*
 * A053283  Coefficients of the '10th-order' mock theta function X(q).
 * https://oeis.org/A053283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k>=0} (-1)^k * coeff of q^{n-k^2} in ∏_{j=1}^{2k}
 *            (1+q^j)^(-1) (coefficient extraction from the generating
 *            function of the 10th‑order mock theta function X(q))
 * Range:     i8 n <= 69, i16 n <= 210, i32 n <= 689, i64 n <= 2422,
 *            i128 n <= 8936, big any n
 * Helpers:   _floor_isqrt_u64, alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/alloc_seq_array.h"

seq_t a(idx_t n)
{
    idx_t max_n = n;
    seq_t *dp = alloc_seq_array(max_n + 1);
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= max_n; ++i)
        dp[i] = (seq_t)0;

    seq_t result = (seq_t)0;
    idx_t kmax = _floor_isqrt_u64(n);

    for (idx_t k = 0; k <= kmax; ++k) {
        if (k > 0) {
            idx_t j1 = 2 * k - 1;
            for (idx_t t = j1; t <= max_n; ++t)
                dp[t] -= dp[t - j1];
            idx_t j2 = 2 * k;
            for (idx_t t = j2; t <= max_n; ++t)
                dp[t] -= dp[t - j2];
        }
        seq_t sign = ((k & 1) == 0) ? (seq_t)1 : (seq_t)-1;
        idx_t tmax = n - k * k;
        if (tmax <= max_n)
            result += sign * dp[tmax];
    }

    free(dp);
    return result;
}
