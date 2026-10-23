/*
 * A170848  G.f.: Product_{k>=1} (1 + 3x^(2^k-1) + 2x^(2^k)).
 * https://oeis.org/A170848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1 + 3x^(2^k−1) + 2x^(2^k))
 *            * Offset: 0
 * Range:     u8 n <= 55, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Convert to size_t for array indexing – the tests use n ≤ 10 000,
       so the cast is safe. */
    size_t N = (size_t)n;

    /* Allocate coefficient arrays.  dp holds the current polynomial,
       temp holds a copy of dp before processing the next factor. */
    seq_t *dp   = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *temp = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!dp || !temp) { /* allocation failure – abort execution */
        free(dp);
        free(temp);
        abort();
    }

    dp[0] = (seq_t)1;             /* initial constant term            */

    /* Process factors (1 + 3x^(2^k-1) + 2x^(2^k)) for k = 1,2,3,… until
       both exponents exceed N. */
    for (idx_t k = 1; ; ++k) {
        idx_t pow2 = ((idx_t)1) << k;   /* 2^k   */
        idx_t e1   = pow2 - 1;          /* 2^k-1 */
        idx_t e2   = pow2;              /* 2^k   */

        if (e1 > n && e2 > n) break;   /* no further contributions */

        /* Copy current polynomial into temp */
        for (size_t i = 0; i <= N; ++i) temp[i] = dp[i];

        /* Add 3 * x^(e1) term */
        if (e1 <= n) {
            size_t shift = (size_t)e1;
            for (size_t i = 0; i + shift <= N; ++i) {
                dp[i + shift] += (seq_t)3 * temp[i];
            }
        }

        /* Add 2 * x^(e2) term */
        if (e2 <= n) {
            size_t shift = (size_t)e2;
            for (size_t i = 0; i + shift <= N; ++i) {
                dp[i + shift] += (seq_t)2 * temp[i];
            }
        }
    }

    seq_t result = dp[N];
    free(dp);
    free(temp);
    return result;
}
