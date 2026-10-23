/*
 * A153360  Number of zig-zag paths from top to bottom of a rectangle of width
 *          10 with n rows.
 * https://oeis.org/A153360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of words of length n over alphabet {1..10} where
 *            each adjacent pair consists of neighboring letters in the
 *            alphabet. Equivalently, the number of walks of length n‑1 on a
 *            path graph of 10 vertices. Recurrence: let dp[i] be the number
 *            of words ending at letter i+1. dp[i] = (i>0?dp[i-1...
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The alphabet has 10 symbols */
    const int W = 10;
    seq_t prev[W], curr[W];
    /* Base case: length 1, exactly one word ending at each letter */
    for (int i = 0; i < W; ++i)
        prev[i] = 1;

    /* For n==1 return 10 directly */
    if (n == 1)
        return 10;

    /* Iterate lengths 2..n */
    for (idx_t len = 2; len <= n; ++len) {
        /* Boundary vertices */
        curr[0] = prev[1];
        curr[W - 1] = prev[W - 2];
        /* Interior vertices */
        for (int i = 1; i < W - 1; ++i)
            curr[i] = prev[i - 1] + prev[i + 1];
        /* swap for next iteration */
        for (int i = 0; i < W; ++i)
            prev[i] = curr[i];
    }

    /* Sum all dp values for the final length */
    seq_t sum = 0;
    for (int i = 0; i < W; ++i)
        sum += prev[i];
    return sum;
}
