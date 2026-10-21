/*
 * A056970  Number of partitions of n into distinct parts congruent to 2, 4 or
 *          5 mod 6.
 * https://oeis.org/A056970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   Uses the generating function ∏_{j≥0} (1 + x^{2+6j})(1 +
 *            x^{4+6j})(1 + x^{5+6j}) to count partitions into distinct parts
 *            ≡ 2, 4 or 5 mod 6
 * Range:     u8 n <= 66, u16 n <= 169, u32 n <= 496, u64 n <= 1620,
 *            u128 n <= 5712, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef __cplusplus
#  include <stddef.h>
#  include <stdlib.h>
#else
#  include <vector>
#  include <cstddef>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++: use a std::vector so each element is default‑constructed. */
    std::vector<seq_t> dp((size_t)n + 1);
    dp[0] = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        if (k % 6 == 2 || k % 6 == 4 || k % 6 == 5) {
            for (idx_t j = n; j >= k; --j) {
                dp[j] += dp[j - k];
            }
        }
    }
    return dp[(size_t)n];
#else
    /* C: allocate an array of seq_t and zero‑initialize it. */
    seq_t *dp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp) return 0;               /* allocation failure */
    dp[0] = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        if (k % 6 == 2 || k % 6 == 4 || k % 6 == 5) {
            for (idx_t j = n; j >= k; --j) {
                dp[j] += dp[j - k];
            }
        }
    }
    seq_t result = dp[(size_t)n];
    free(dp);
    return result;
#endif
}
