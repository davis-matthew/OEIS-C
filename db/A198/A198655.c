/*
 * A198655  Number of n X 1 0..7 arrays with values 0..7 introduced in row
 *          major order and each element equal to at least one horizontal or
 *          vertical neighbor.
 * https://oeis.org/A198655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 28, u64 n <= 47, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A198655 – Number of n X 1 0..7 arrays with values 0..7 introduced in row major order
    and each element equal to at least one horizontal or vertical neighbor.
    The implementation uses a recurrence based on run-length description
    with the “first‑appearance order” constraint.  The sequence is
    computed by dynamic programming in O(n) time and O(1) memory
    (apart from the output type).  Initial terms are
    a(1)=0, a(2)=1, a(3)=1, a(4)=2, a(5)=3, a(6)=6, a(7)=11, ...  */
seq_t a(idx_t n)
{
    /* Handle the trivial cases.  The offset is 1, so a(1)=0.  */
    if (n == 0) return ((seq_t)1);      /* only for completeness */
    if (n == 1) return ((seq_t)0);

    /* 0 ≤ k ≤ 8 (number of distinct symbols used so far).
       For each k the index “prev” ranges over 0..k where
       prev==k indicates that no run has been started yet
       (i.e., there is no previous symbol).  */
    const int KMAX = 8;

    /* DP arrays for remaining length “rem”.  dp_prev2 holds
       the values for rem-2, dp_prev1 for rem-1,
       dp_cur   for the current rem.  */
    static seq_t dp_prev2[9][9];
    static seq_t dp_prev1[9][9];
    static seq_t dp_cur  [9][9];

    /* S arrays store cumulative sums
       S[k][prev][rem] = Σ_{t=0}^{rem-2} dp[k][prev][t]. */
    static seq_t S_prev[9][9];
    static seq_t S_cur [9][9];

    /* Initialise for rem = 0 (empty array).  */
    for (int k = 0; k <= KMAX; ++k)
        for (int prev = 0; prev <= k; ++prev)
            dp_prev2[k][prev] = (seq_t)1;      /* one way to fill length 0 */

    /* Initialise for rem = 1 (no valid array of length 1).  */
    for (int k = 0; k <= KMAX; ++k)
        for (int prev = 0; prev <= k; ++prev)
            dp_prev1[k][prev] = (seq_t)0;

    /* S arrays start at 0 for all entries.  */
    for (int k = 0; k <= KMAX; ++k)
        for (int prev = 0; prev <= k; ++prev)
            S_prev[k][prev] = (seq_t)0;

    /* Main dynamic‑programming loop.  We start from rem = 2 because
       a(1) is already handled.  */
    for (idx_t rem = 2; rem <= n; ++rem)
    {
        /* Compute S_cur from the previous S and dp_prev2 (rem-2).  */
        for (int k = 0; k <= KMAX; ++k)
            for (int prev = 0; prev <= k; ++prev)
                S_cur[k][prev] = S_prev[k][prev] + dp_prev2[k][prev];

        /* Compute dp_cur for the current rem.  */
        for (int k = 0; k <= KMAX; ++k)
        {
            for (int prev = 0; prev <= k; ++prev)
            {
                seq_t sum = (seq_t)0;
                /* Start a new symbol if allowed.  */
                if (k < KMAX)            /* a new symbol with value k */
                    sum += S_cur[k + 1][k];
                /* Start an old symbol different from the previous one.  */
                if (k > 0)
                {
                    for (int s = 0; s < k; ++s)
                        if (s != prev)
                            sum += S_cur[k][s];
                }
                dp_cur[k][prev] = sum;
            }
        }

        /* Prepare for the next iteration.  */
        for (int k = 0; k <= KMAX; ++k)
            for (int prev = 0; prev <= k; ++prev)
            {
                dp_prev2[k][prev] = dp_prev1[k][prev];
                dp_prev1[k][prev] = dp_cur  [k][prev];
                S_prev   [k][prev] = S_cur  [k][prev];
            }
    }

    /* After the loop, dp_cur[0][0] holds a(n).  */
    return dp_cur[0][0];
}
