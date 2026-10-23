/*
 * A161875  Number of reduced words of length n in the Weyl group B_15.
 * https://oeis.org/A161875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is the coefficient of x^n in ∏_{k=1..15} (1–x^(2k))/(1–x)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 23, u64 n <= 225, u128 n <= 225,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n > 225 the coefficient is 0. */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The maximum degree of the polynomial is 15^2 = 225. */
    static seq_t dp[226];
    static bool initialized = false;
    const size_t m = 15;
    if (!initialized) {
        /* Initialize dp[0] = 1 and the rest to 0 (static array is zeroed). */
        dp[0] = 1;
        size_t max_deg = 0;  /* current maximum degree of dp */

        for (size_t k = 1; k <= m; ++k) {
            /* The factor (1 - x^(2k))/(1 - x) = 1 + x + ... + x^(2k-1). */
            const size_t L = 2 * k - 1;          /* number of terms in the factor minus 1 */
            const size_t new_deg = max_deg + L;  /* new maximum degree */

            /* Prefix sums of the current dp to enable O(1) range sum queries. */
            seq_t pref[max_deg + 2];
            pref[0] = 0;
            for (size_t i = 0; i <= max_deg; ++i)
                pref[i + 1] = pref[i] + dp[i];

            /* Compute the new polynomial coefficients. */
            seq_t ndp[226];  /* size is enough because new_deg <= 225 */
            for (size_t i = 0; i <= new_deg; ++i) {
                const size_t low  = (i > L)     ? (i - L) : 0;
                const size_t high = (i <= max_deg) ? i : max_deg;
                ndp[i] = pref[high + 1] - pref[low];
            }

            /* Copy back to dp and update the current maximum degree. */
            for (size_t i = 0; i <= new_deg; ++i)
                dp[i] = ndp[i];
            max_deg = new_deg;
        }
        initialized = true;
    }

    if (n <= 225ULL) {
        return dp[(size_t)n];
    } else {
        return 0;
    }
}
