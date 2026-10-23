/*
 * A126541  Number of base 20 n-digit numbers with adjacent digits differing
 *          by five or less.
 * https://oeis.org/A126541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=20.
   For n ≥ 2 a(n) is obtained by dynamic programming over
   the 20 digits 0..19.  Let f(m,d) be the number of
   length‑m strings ending in digit d.  Then
     f(1,d) = 1                          (every single digit)
     f(m+1,d) = Σ_{k, |k-d|≤5} f(m,k).
   Finally a(n) = Σ_{d=0}^{19} f(n,d).   */

seq_t a(idx_t n)
{
    /* 0‑digit strings: empty string */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Use small constants for readability */
    const unsigned int DIGITS = 20;   /* digits 0 … 19 */
    const unsigned int RADIUS = 5;    /* |k-d| ≤ 5 */

    /* dp[d] stores f(m,d) for current length m */
    seq_t dp[20];
    seq_t newdp[20];

    /* length 1: each digit appears once */
    for (idx_t d = 0; d < DIGITS; ++d) {
        dp[d] = (seq_t)1;
    }

    /* Build up to length n */
    for (idx_t len = 2; len <= n; ++len) {
        for (idx_t d = 0; d < DIGITS; ++d) {
            seq_t sum = (seq_t)0;
            idx_t start = (d > RADIUS) ? (d - RADIUS) : (idx_t)0;
            idx_t end   = (d + RADIUS < DIGITS - 1) ? (d + RADIUS) : (idx_t)(DIGITS - 1);
            for (idx_t k = start; k <= end; ++k) {
                sum += dp[k];
            }
            newdp[d] = sum;
        }
        /* swap dp and newdp */
        for (idx_t d = 0; d < DIGITS; ++d) {
            dp[d] = newdp[d];
        }
    }

    /* Sum over all ending digits */
    seq_t total = (seq_t)0;
    for (idx_t d = 0; d < DIGITS; ++d) {
        total += dp[d];
    }
    return total;
}
