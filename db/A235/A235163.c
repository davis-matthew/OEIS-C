/*
 * A235163  Number of positive integers with n digits in which adjacent digits
 *          differ by at most 1.
 * https://oeis.org/A235163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: u(1,0) = 0, u(1,r) = 1 for 1 <= r <= 9;
   for n > 1: u(n,r) = u(n-1,r-1) + u(n-1,r) + u(n-1,r+1),
   where u(n,r) = 0 for r < 0 or r > 9.
   a(n) = sum_{r=0..9} u(n,r).  */

/* Compute the number of n‑digit positive integers whose adjacent
   digits differ by at most 1. */
seq_t a(idx_t n)
{
    /* n >= 1 by the problem statement. */
    seq_t dp[10];
    seq_t next[10];
    idx_t i, r;

    /* Base case: n == 1 */
    dp[0] = 0;
    for (r = 1; r <= 9; ++r)
        dp[r] = 1;

    if (n == 1) {
        seq_t total = 0;
        for (r = 0; r <= 9; ++r)
            total += dp[r];
        return total;
    }

    /* Iterate for lengths 2 .. n */
    for (i = 1; i < n; ++i) {
        /* Compute next[0] and next[9] separately */
        next[0] = dp[0] + dp[1];
        next[9] = dp[8] + dp[9];

        /* Compute next[1]..next[8] */
        for (r = 1; r <= 8; ++r)
            next[r] = dp[r-1] + dp[r] + dp[r+1];

        /* Copy next to dp for the next iteration */
        for (r = 0; r <= 9; ++r)
            dp[r] = next[r];
    }

    /* Sum all u(n,r) to obtain a(n) */
    seq_t total = 0;
    for (r = 0; r <= 9; ++r)
        total += dp[r];
    return total;
}
