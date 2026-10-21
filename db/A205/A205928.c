/*
 * A205928  Number of (n+1)X2 0..3 arrays with the number of clockwise edge
 *          increases in 2X2 subblocks nondecreasing rightwards and downwards.
 * https://oeis.org/A205928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×2 arrays over {0,1,2,3} whose clockwise
 *            edge‑increase counts in each 2×2 subblock are nondecreasing.
 *            Computed by dynamic programming on row pairs
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Precompute the clockwise‑edge‑increase count for every pair of rows.
       A row is encoded as a two‑digit base‑4 number (0..15):
       digit 0 = value in column 0, digit 1 = value in column 1. */
    static int inc[16][16];
    static int inc_init = 0;
    if (!inc_init) {
        for (int p = 0; p < 16; ++p) {
            int p0 = p / 4;
            int p1 = p % 4;
            for (int c = 0; c < 16; ++c) {
                int c0 = c / 4;
                int c1 = c % 4;
                int cnt = 0;
                if (p0 < p1) ++cnt;
                if (p1 < c1) ++cnt;
                if (c1 < c0) ++cnt;
                if (c0 < p0) ++cnt;
                inc[p][c] = cnt;
            }
        }
        inc_init = 1;
    }

    /* DP over rows.
       dp[cur][r][k] = number of partial arrays ending at the previous
       row with state r (0..15) and last clockwise‑edge‑increase count k (0..4). */
    static seq_t dp[2][16][5];
    int cur = 0, nxt = 1;

    /* Initial state: before any subblock the last count is 0.
       Each possible first row contributes one way. */
    for (int r = 0; r < 16; ++r) {
        for (int k = 0; k < 5; ++k) dp[cur][r][k] = 0;
        dp[cur][r][0] = 1;
    }

    /* Perform n transitions (one for each 2×2 subblock). */
    for (idx_t step = 1; step <= n; ++step) {
        /* Reset next layer. */
        for (int r = 0; r < 16; ++r)
            for (int k = 0; k < 5; ++k)
                dp[nxt][r][k] = 0;

        /* Transition. */
        for (int rPrev = 0; rPrev < 16; ++rPrev) {
            for (int last = 0; last < 5; ++last) {
                seq_t ways = dp[cur][rPrev][last];
                if (!ways) continue;
                for (int rNext = 0; rNext < 16; ++rNext) {
                    int incCnt = inc[rPrev][rNext];
                    if (incCnt >= last)
                        dp[nxt][rNext][incCnt] += ways;
                }
            }
        }

        /* Swap current and next layers. */
        int tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* Sum all ways for the final row. */
    seq_t result = 0;
    for (int r = 0; r < 16; ++r)
        for (int k = 0; k < 5; ++k)
            result += dp[cur][r][k];

    return result;
}
