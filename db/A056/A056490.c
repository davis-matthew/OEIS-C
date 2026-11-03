/*
 * A056490  Number of periodic palindromes using exactly four different
 *          symbols.
 * https://oeis.org/A056490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12 * (S2(floor((n+1)/2),4) + S2(ceil((n+1)/2),4))
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 620 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where S2(m,4) is the Stirling number of the second kind, 4‑th column. */

#include <stdbool.h>

static seq_t S2_four(idx_t m)
{
    /* Compute S(m,4) for m >= 0 using the recurrence
       S(n,k) = k * S(n-1,k) + S(n-1,k-1)  (S(0,0)=1, S(0,k)=0 for k>0). */
    seq_t dp[5] = {0};
    dp[0] = 1;                     /* S(0,0) */

    for (idx_t i = 1; i <= m; ++i) {
        seq_t newdp[5];
        newdp[0] = 0;              /* S(i,0) = 0 for i > 0 */
        for (int k = 1; k <= 4; ++k) {
            newdp[k] = (seq_t)k * dp[k] + dp[k - 1];
        }
        for (int k = 0; k <= 4; ++k) {
            dp[k] = newdp[k];
        }
    }
    return dp[4];
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;                 /* safety for n < 1 */
    }

    idx_t total = n + 1;          /* (n+1) */
    idx_t m1 = total >> 1;        /* floor((n+1)/2) */
    idx_t m2 = total - m1;        /* ceil((n+1)/2)  */

    seq_t s1 = S2_four(m1);
    seq_t s2 = S2_four(m2);

    return (seq_t)12 * (s1 + s2);
}
