/*
 * A224192  Number of 5Xn 0..2 arrays with rows unimodal and columns
 *          nondecreasing.
 * https://oeis.org/A224192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 46, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of 5×n arrays over {0,1,2}
 * with each column nondecreasing and each row unimodal
 * (first nondecreasing, then nonincreasing, possibly monotone).
 * Computed by dynamic programming over column types and row‑phase states.
 */

seq_t a(idx_t n) {
    if (n < 1)
        return 0;

    /* ---------- Precomputation (once) ---------- */
    static int initialized = 0;
    static int col[21][5];                    /* all 21 nondecreasing 5‑tuples */
    static short trans[21 * 32][21];           /* transition table */

    if (!initialized) {
        /* Enumerate the 21 possible columns: 0 ≤ a ≤ b ≤ c ≤ d ≤ e ≤ 2 */
        int t = 0;
        for (int a = 0; a <= 2; ++a)
            for (int b = a; b <= 2; ++b)
                for (int c = b; c <= 2; ++c)
                    for (int d = c; d <= 2; ++d)
                        for (int e = d; e <= 2; ++e) {
                            col[t][0] = a;
                            col[t][1] = b;
                            col[t][2] = c;
                            col[t][3] = d;
                            col[t][4] = e;
                            ++t;                 /* t runs 0..20 */
                        }

        const int STATES = 21 * 32;           /* 672 */

        for (int phase = 0; phase < 32; ++phase) {
            for (int ci = 0; ci < 21; ++ci) {
                int cur_state = phase * 21 + ci;
                for (int nxt = 0; nxt < 21; ++nxt) {
                    int new_phase = phase;
                    int ok = 1;
                    for (int r = 0; r < 5 && ok; ++r) {
                        int prev = col[ci][r];
                        int next = col[nxt][r];
                        if (phase & (1 << r)) {           /* already decreasing */
                            if (next > prev)               /* would increase again */
                                ok = 0;
                        } else {                            /* still increasing */
                            if (next < prev)
                                new_phase |= 1 << r;       /* start decreasing */
                        }
                    }
                    if (ok)
                        trans[cur_state][nxt] = (short)(new_phase * 21 + nxt);
                    else
                        trans[cur_state][nxt] = -1;
                }
            }
        }
        initialized = 1;
    }

    const int STATES = 21 * 32;

    static seq_t dp[672];
    static seq_t ndp[672];

    /* initialize dp for the first column (phase=0 for all rows) */
    for (int s = 0; s < STATES; ++s)
        dp[s] = 0;
    for (int c = 0; c < 21; ++c)
        dp[c] = 1;                         /* state (c,0) */

    if (n == 1) {
        seq_t tot = 0;
        for (int s = 0; s < STATES; ++s)
            tot += dp[s];
        return tot;
    }

    /* iterate over remaining columns */
    for (idx_t step = 1; step < n; ++step) {
        for (int s = 0; s < STATES; ++s)
            ndp[s] = 0;
        for (int s = 0; s < STATES; ++s) {
            if (dp[s] == 0)
                continue;
            for (int nxt = 0; nxt < 21; ++nxt) {
                short nxt_state = trans[s][nxt];
                if (nxt_state >= 0)
                    ndp[nxt_state] += dp[s];
            }
        }
        for (int s = 0; s < STATES; ++s)
            dp[s] = ndp[s];
    }

    /* sum all end states */
    seq_t ans = 0;
    for (int s = 0; s < STATES; ++s)
        ans += dp[s];
    return ans;
}
