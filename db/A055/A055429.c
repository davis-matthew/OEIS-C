/*
 * A055429  Number of points in Z^n of norm <= 5.
 * https://oeis.org/A055429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of integer n‑tuples with sum of squares <= 25
 *            (points of ℤⁿ of Euclidean norm ≤ 5)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 14, u64 n <= 38, u128 n <= 188,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case for n = 0.  The empty tuple has norm 0. */
    if (n == 0)
        return (seq_t)1;

    /* dp[t] = number of (dim‑1)-tuples with sum of squares = t.
       ndp will hold the values for dim tuples. */
    seq_t dp[26] = {0};
    seq_t ndp[26] = {0};

    dp[0] = (seq_t)1;         /* 0‑dimensional tuple has sum 0 */

    for (idx_t dim = 1; dim <= n; ++dim) {
        /* Reset ndp for this dimension. */
        for (int t = 0; t <= 25; ++t)
            ndp[t] = (seq_t)0;

        /* For each possible sum of squares t (0..25), accumulate
           contributions from all possible coordinate values a. */
        for (int t = 0; t <= 25; ++t) {
            seq_t sum = (seq_t)0;
            for (int a = 0; a <= 5; ++a) {
                int a2 = a * a;
                if (a2 > t)
                    break;
                if (a == 0)
                    sum += dp[t - a2];
                else {
                    sum += dp[t - a2];
                    sum += dp[t - a2];   /* two signs for a ≠ 0 */
                }
            }
            ndp[t] = sum;
        }

        /* Copy results back to dp for the next iteration. */
        for (int t = 0; t <= 25; ++t)
            dp[t] = ndp[t];
    }

    /* The desired count is the sum over all sum‑of‑squares values ≤ 25. */
    seq_t total = (seq_t)0;
    for (int t = 0; t <= 25; ++t)
        total += dp[t];

    return total;
}
