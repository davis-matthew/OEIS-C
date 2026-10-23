/*
 * A029120  Expansion of 1/((1-x)*(1-x^7)*(1-x^8)*(1-x^12)).
 * https://oeis.org/A029120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 627, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A029120 – number of partitions of n into parts 1, 7, 8, 12
    Implementation uses the classic one‑dimensional coin‑change DP.
    For each coin f ∈ {1,7,8,12} the recurrence
          dp[s] += dp[s-f]   (for all s ≥ f)
    yields the desired counts.  The result is correct for any n ≥ 0
    and for any seq_t / idx_t type.  The function allocates a
    temporary vector/array that is freed before returning.  It
    supports both C11 and C++17 builds. */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Parts allowed in the partitions */
static const idx_t _parts[4] = {1, 7, 8, 12};

seq_t a(idx_t n)
{
    /* Allocate a temporary array/vector for the DP */
#ifdef __cplusplus
    std::vector<seq_t> dp(n + 1);
#else
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0;          /* allocation failed */
#endif

    /* Initialise entries */
    for (idx_t i = 0; i <= n; ++i) {
#ifdef __cplusplus
        dp[i] = 0;
#else
        dp[i] = 0;
#endif
    }
    dp[0] = 1;                   /* empty partition */

    /* Apply the coin‑change recurrence for each part */
    for (int k = 0; k < 4; ++k) {
        idx_t coin = _parts[k];
        for (idx_t s = coin; s <= n; ++s) {
            dp[s] += dp[s - coin];
        }
    }

    /* Get the result and clean up */
    seq_t result = dp[n];
#ifdef __cplusplus
    /* vector destructor cleans up automatically */
#else
    free(dp);
#endif
    return result;
}
