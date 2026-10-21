/*
 * A079924  Solution to the Dancing School Problem with n girls and n+5 boys:
 *          f(n,5).
 * https://oeis.org/A079924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 18, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of perfect matchings in the (0,1)-matrix B of size n × (n+5) where
 * B[i][j] = 1 iff i ≤ j ≤ i+5.
 * This is equivalent to the number of injective mappings π:[n]→[n+5]
 * with π(i)∈[i,i+5].  The value is returned in seq_t.
 */
seq_t a(idx_t n)
{
    /* for n < 1 we do not compute a value (offset is 1) */
    if (n == 0) return 0U;

    /* window width: the number of columns currently relevant to a row */
    const int W = 6;           /* 1 + 5 */
    const int MASKS = 1 << W;  /* 64 states (0..63) */

    seq_t dp[MASKS];
    seq_t ndp[MASKS];

    /* initialise dp[0] = 1, others 0 */
    for (int m = 0; m < MASKS; ++m) dp[m] = 0;
    dp[0] = 1;

    for (idx_t row = 1; row <= n; ++row) {
        /* clear ndp */
        for (int m = 0; m < MASKS; ++m) ndp[m] = 0;

        for (int m = 0; m < MASKS; ++m) {
            seq_t ways = dp[m];
            if (ways == 0) continue;

            /* choose a column c in the current window (0..5) that is not yet used */
            for (int c = 0; c < W; ++c) {
                if (m & (1 << c)) continue;     /* column is already occupied */

                /* shift window for the next row: drop the leftmost column */
                int mnext = m >> 1;              /* drop bit 0, shift others right */

                /* if we chose a column other than the leftmost one, mark it
                 * as occupied in the new window (relative index c-1). */
                if (c > 0) mnext |= (1 << (c - 1));

                ndp[mnext] += ways;
            }
        }

        /* move ndp to dp for the next iteration */
        for (int m = 0; m < MASKS; ++m) dp[m] = ndp[m];
    }

    /* sum all ending states */
    seq_t result = 0;
    for (int m = 0; m < MASKS; ++m) result += dp[m];
    return result;
}
