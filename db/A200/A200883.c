/*
 * A200883  Number of 0..5 arrays x(0..n+1) of n+2 elements without any
 *          interior element greater than both neighbors.
 * https://oeis.org/A200883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A200883
    a(n) = Number of length-(n+2) arrays over {0..5} with no
    interior element greater than both neighbors.
    Computed by a 36-state transfer-matrix DP. */

seq_t a(idx_t n)
{
    const int K = 6;                      // alphabet size
    const int S = K*K;                    // number of states (prev,curr)

    if (n == (idx_t)0) return 0;          // not defined for n<1

    idx_t L = n + 2;                      // sequence length

    /* dp[state] = number of ways to have the last two values equal
       to (prev,curr) after the current step. */
    seq_t dp[S], next[S];
    for (int i = 0; i < S; ++i) dp[i] = 1;      // all pairs possible for length 2

    /* For every new position from 2 to L-1 add one more digit. */
    for (idx_t pos = 2; pos <= L-1; ++pos) {
        for (int i = 0; i < S; ++i) next[i] = 0;

        for (int prev = 0; prev < K; ++prev) {
            for (int curr = 0; curr < K; ++curr) {
                seq_t cnt = dp[prev*K + curr];
                if (!cnt) continue;
                for (int nxt = 0; nxt < K; ++nxt) {
                    if (curr > prev && curr > nxt) continue;  /* forbidden local max */
                    next[curr*K + nxt] += cnt;
                }
            }
        }

        for (int i = 0; i < S; ++i) dp[i] = next[i];
    }

    /* Sum all end states to obtain the total number of arrays. */
    seq_t total = 0;
    for (int i = 0; i < S; ++i) total += dp[i];
    return total;
}
