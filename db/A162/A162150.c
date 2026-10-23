/*
 * A162150  Number of reduced words of length n in the Weyl group B_36.
 * https://oeis.org/A162150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in Prod_{k=1..36} (1 + x + ... +
 *            x^{2k-1}) (Weyl group B_36)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define MAX_DEG 1296  /* 36^2 */

seq_t a(idx_t n)
{
    static seq_t dp[MAX_DEG + 1];
    static seq_t ndp[MAX_DEG + 1];
    static bool initialized = false;

    if (!initialized)
    {
        /* initialise the polynomial to 1 */
        for (idx_t i = 0; i <= MAX_DEG; ++i)
            dp[i] = 0;
        dp[0] = 1;

        seq_t *cur  = dp;
        seq_t *next = ndp;

        for (idx_t k = 1; k <= 36; ++k)
        {
            idx_t L      = 2 * k;      /* length of current factor */
            idx_t new_deg = k * k;     /* degree after this factor */
            seq_t window = 0;

            for (idx_t i = 0; i <= new_deg; ++i)
            {
                window += cur[i];
                if (i >= L)
                    window -= cur[i - L];
                next[i] = window;
            }

            /* clear remaining coefficients to avoid stale values */
            for (idx_t i = new_deg + 1; i <= MAX_DEG; ++i)
                next[i] = 0;

            /* swap buffers for next iteration */
            seq_t *tmp = cur;
            cur = next;
            next = tmp;
        }

        /* after 36 even swaps, dp contains the final coefficients */
        initialized = true;
    }

    return (n > MAX_DEG) ? 0 : dp[n];
}
