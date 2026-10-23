/*
 * A035941  Number of partitions of n into parts not of the form 9k, 9k+2 or
 *          9k-2. Also number of partitions with 1 part of size 1 and
 *          differences between parts at distance 3 are greater than 1.
 * https://oeis.org/A035941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts not congruent to
 *            0,2,7 modulo 9
 * Range:     u8 n <= 22, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for n >= 1.
   Uses a standard coin‑change DP where the allowed “coins” are all integers
   whose residue mod 9 is 1,3,4,5,6, or 8. */
seq_t a(idx_t n) {
    if (n == 0) return 0;               /* defined only for n >= 1, but handle safely */

    /* Allocate DP array of size n+1 */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0;                  /* out of memory – should not happen */

    /* Initialise DP */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;                          /* one way to make 0 */

    /* Process each allowed part size */
    for (idx_t part = 1; part <= n; ++part) {
        idx_t r = part % 9;
        if (r == 0 || r == 2 || r == 7) continue;   /* disallowed residue */
        for (idx_t s = part; s <= n; ++s) {
            dp[s] = dp[s] + dp[s - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
