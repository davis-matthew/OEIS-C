/*
 * A126534  Number of base 13 n-digit numbers with adjacent digits differing
 *          by five or less.
 * https://oeis.org/A126534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; a(1)=13.  
   For each length n≥2 use dynamic programming on the last digit:
   let dp[d] be the number of length‑current sequences ending with digit d (0≤d<13).  
   Initially all dp[d] = 1.  
   For each subsequent position:
       newdp[d] = Σ_{k=max(0,d‑5)}^{min(12,d+5)} dp[k];
   After n‑1 transitions dp holds counts for length n.  
   The desired value a(n) is Σ_{d=0}^{12} dp[d]. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;          /* a(0) = 1 */

    seq_t dp[13];
    seq_t newdp[13];
    for (int d = 0; d < 13; ++d)
        dp[d] = (seq_t)1;          /* a(1) = 13, all digits allowed */

    for (idx_t len = 1; len < n; ++len) {
        for (int d = 0; d < 13; ++d) {
            seq_t sum = (seq_t)0;
            int lo = d - 5;
            if (lo < 0) lo = 0;
            int hi = d + 5;
            if (hi > 12) hi = 12;
            for (int k = lo; k <= hi; ++k)
                sum += dp[k];
            newdp[d] = sum;
        }
        for (int d = 0; d < 13; ++d)
            dp[d] = newdp[d];
    }

    seq_t total = (seq_t)0;
    for (int d = 0; d < 13; ++d)
        total += dp[d];
    return total;                    /* a(n) */
}
