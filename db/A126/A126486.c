/*
 * A126486  Number of base 18 n-digit numbers with adjacent digits differing
 *          by three or less.
 * https://oeis.org/A126486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A126486: Number of base‑18 n‑digit numbers whose adjacent digits differ by ≤3.
   a(0) = 1
   For n≥1, let dp[d] store the number of valid n‑digit sequences ending with digit d (0‑17).
   Initialisation: dp[d] = 1 for all d (there is exactly one 1‑digit number ending with d).
   Transition: dp_next[d] = Σ_{e: |e−d|≤3} dp[e]
   After n‑1 transitions, a(n) = Σ_{d=0}^{17} dp[d].
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0U) {
        return (seq_t)1;          /* the empty string */
    }

    /* dp[d] holds counts for current length; dp_next[d] for next length */
    seq_t dp[18];
    seq_t dp_next[18];

    /* initialise for length 1: exactly one number ending with each digit */
    for (int d = 0; d < 18; ++d) {
        dp[d] = (seq_t)1;
    }

    /* iterate from length 2 up to n */
    for (idx_t len = 2U; len <= n; ++len) {
        for (int d = 0; d < 18; ++d) {
            seq_t s = (seq_t)0;
            int low  = d - 3; if (low < 0)       low = 0;
            int high = d + 3; if (high > 17)    high = 17;
            for (int e = low; e <= high; ++e) {
                s += dp[e];
            }
            dp_next[d] = s;
        }
        /* copy dp_next back to dp for next iteration */
        for (int d = 0; d < 18; ++d) {
            dp[d] = dp_next[d];
        }
    }

    /* sum over all ending digits to obtain a(n) */
    seq_t total = (seq_t)0;
    for (int d = 0; d < 18; ++d) {
        total += dp[d];
    }

    return total;
}
