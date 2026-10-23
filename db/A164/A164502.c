/*
 * A164502  Number of binary strings of length n with no substrings equal to
 *          0010 1011 or 1100.
 * https://oeis.org/A164502
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Number of binary strings of length n with no substrings equal to
 *            0010, * 1011, or 1100 (OEIS A164502). Computed via a simple DFA
 *            built from the * forbidden patterns
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 48, u64 n <= 99, u128 n <= 201,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Automaton data (fixed size, small patterns). */
    static int next_state[20][2];
    static int fail[20];
    static bool bad_state[20];
    static int state_cnt;
    static bool init_done = false;

    /* Build the automaton once. */
    if (!init_done) {
        /* Initialize all nodes. */
        for (int i = 0; i < 20; ++i) {
            next_state[i][0] = next_state[i][1] = -1;
            fail[i] = 0;
            bad_state[i] = false;
        }
        state_cnt = 1;                /* root is node 0 */

        /* Forbidden substrings. */
        const char *patterns[] = { "0010", "1011", "1100" };
        const int pat_cnt = 3;

        for (int p = 0; p < pat_cnt; ++p) {
            int v = 0;
            const char *s = patterns[p];
            for (; *s; ++s) {
                int c = *s - '0';
                if (next_state[v][c] == -1) {
                    next_state[v][c] = state_cnt;
                    next_state[state_cnt][0] = next_state[state_cnt][1] = -1;
                    fail[state_cnt] = 0;
                    bad_state[state_cnt] = false;
                    ++state_cnt;
                }
                v = next_state[v][c];
            }
            bad_state[v] = true;
        }

        /* Build failure links (BFS). */
        int q[20];
        int head = 0, tail = 0;
        for (int c = 0; c < 2; ++c) {
            int u = next_state[0][c];
            if (u != -1) {
                fail[u] = 0;
                q[tail++] = u;
            } else {
                next_state[0][c] = 0;
            }
        }

        while (head < tail) {
            int v = q[head++];
            bad_state[v] = bad_state[v] || bad_state[fail[v]];
            for (int c = 0; c < 2; ++c) {
                int u = next_state[v][c];
                if (u != -1) {
                    fail[u] = next_state[fail[v]][c];
                    q[tail++] = u;
                } else {
                    next_state[v][c] = next_state[fail[v]][c];
                }
            }
        }
        init_done = true;
    }

    if (n < 4)
        return 0;   /* offset is 4; values below are irrelevant */

    /* DP arrays for current and next state counts. */
    seq_t dp[20], ndp[20];
    for (int s = 0; s < state_cnt; ++s)
        dp[s] = 0;
    dp[0] = 1;   /* empty string starts at root */

    for (idx_t pos = 0; pos < n; ++pos) {
        for (int s = 0; s < state_cnt; ++s)
            ndp[s] = 0;
        for (int s = 0; s < state_cnt; ++s) {
            if (bad_state[s]) continue;
            seq_t cur = dp[s];
            if (cur == 0) continue;
            for (int b = 0; b < 2; ++b) {
                int ns = next_state[s][b];
                if (!bad_state[ns]) {
                    ndp[ns] += cur;
                }
            }
        }
        for (int s = 0; s < state_cnt; ++s)
            dp[s] = ndp[s];
    }

    seq_t ans = 0;
    for (int s = 0; s < state_cnt; ++s)
        if (!bad_state[s])
            ans += dp[s];

    return ans;
}
