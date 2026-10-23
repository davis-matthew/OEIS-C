/*
 * A162193  Number of reduced words of length n in the Weyl group B_48.
 * https://oeis.org/A162193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1}^{48}(1 + x + ... +
 *            x^{2k-1})
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e.   a(n) is the number of reduced words of length n in the Weyl group B_48 */

#include <stdbool.h>

#define MAX_DEG 2304   /* sum_{k=1}^{48} (2k-1) = 48*48 */

seq_t a(idx_t n)
{
    static seq_t dp[MAX_DEG + 1];
    static seq_t dp_new[MAX_DEG + 1];
    static seq_t prefix[MAX_DEG + 1];
    static bool initialized = false;
    if (!initialized) {
        /* dp[0] = 1, all others 0 */
        for (idx_t i = 0; i <= MAX_DEG; ++i) dp[i] = 0;
        dp[0] = 1;
        idx_t cur_max = 0;
        for (idx_t k = 1; k <= 48; ++k) {
            idx_t block_len = 2 * k - 1;      /* length of factor polynomial */
            idx_t new_max = cur_max + block_len;
            /* compute prefix sums of current dp */
            prefix[0] = dp[0];
            for (idx_t i = 1; i <= cur_max; ++i)
                prefix[i] = prefix[i - 1] + dp[i];
            /* compute dp_new[n] = prefix[n] - prefix[n-2k]  (for n>=2k) */
            for (idx_t n_i = 0; n_i <= new_max; ++n_i) {
                seq_t s = 0;
                if (n_i <= cur_max)
                    s = prefix[n_i];
                else
                    s = prefix[cur_max];
                if (n_i >= 2 * k) {
                    seq_t sub = prefix[n_i - 2 * k];
                    s = s - sub;  /* prefix[n] >= prefix[n-2k], so non‑negative */
                }
                dp_new[n_i] = s;
            }
            /* swap dp and dp_new for next iteration */
            for (idx_t i = 0; i <= new_max; ++i)
                dp[i] = dp_new[i];
            cur_max = new_max;
        }
        initialized = true;
    }
    if (n <= MAX_DEG)
        return dp[n];
    else
        return 0;
}
