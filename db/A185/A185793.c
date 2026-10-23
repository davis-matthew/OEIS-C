/*
 * A185793  Number of (n+1)X5 binary arrays with no 2X2 subblock sum equal to
 *          any horizontal or vertical neighbor 2X2 subblock sum.
 * https://oeis.org/A185793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 18, u64 n <= 41, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A185793: Number of (n+1)×5 binary arrays with no adjacent 2×2 subblock having equal sums.
   Computed via dynamic programming over last two rows.
   State: pair of consecutive rows (0..31 each). Transition allowed only if
   horizontal sums within a pair differ consecutively and vertical sums
   between successive pairs differ in all four columns.  */

/* Includes required for bool in C. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Helper data structures; computed once. */
    static bool initialized = false;
    static bool horiz_ok[32][32];
    static unsigned char sumvec[32][32][4];
    static bool valid_state[1024];
    static unsigned char next_cnt[1024];
    static int next_state[1024][32];

    if (!initialized) {
        initialized = true;
        /* Pre‑compute row‑pair properties. */
        for (int a = 0; a < 32; ++a) {
            for (int b = 0; b < 32; ++b) {
                unsigned char s[4];
                for (int j = 0; j < 4; ++j) {
                    int bits = ((a >> j) & 1) + ((a >> (j + 1)) & 1)
                             + ((b >> j) & 1) + ((b >> (j + 1)) & 1);
                    s[j] = (unsigned char)bits;          /* 0..4 */
                }
                bool h_ok = (s[0] != s[1]) && (s[1] != s[2]) && (s[2] != s[3]);
                horiz_ok[a][b] = h_ok;
                if (h_ok) {
                    for (int j = 0; j < 4; ++j) sumvec[a][b][j] = s[j];
                } else {
                    for (int j = 0; j < 4; ++j) sumvec[a][b][j] = 0;
                }
            }
        }

        /* Build adjacency lists for DP transitions. */
        for (int a = 0; a < 32; ++a) {
            for (int b = 0; b < 32; ++b) {
                int idx = (a << 5) | b;
                if (!horiz_ok[a][b]) {
                    valid_state[idx] = false;
                    next_cnt[idx] = 0;
                    continue;
                }
                valid_state[idx] = true;
                int cnt = 0;
                for (int c = 0; c < 32; ++c) {
                    if (!horiz_ok[b][c]) continue;
                    bool vert_ok = true;
                    for (int j = 0; j < 4; ++j) {
                        if (sumvec[a][b][j] == sumvec[b][c][j]) {
                            vert_ok = false;
                            break;
                        }
                    }
                    if (vert_ok) {
                        next_state[idx][cnt++] = (b << 5) | c;
                    }
                }
                next_cnt[idx] = cnt;
            }
        }
    }

    /* DP arrays.  Use stack arrays because size is modest (1024). */
    seq_t dp[1024] = {0};
    seq_t newdp[1024] = {0};

    /* Base case: sequences of two rows.  a(1) */
    for (int i = 0; i < 1024; ++i) {
        if (valid_state[i]) dp[i] = 1;
    }

    /* Transition for each additional row (n-1 steps). */
    for (idx_t step = 1; step < n; ++step) {
        /* Zero newdp. */
        for (int i = 0; i < 1024; ++i) newdp[i] = 0;

        /* Process all current states. */
        for (int i = 0; i < 1024; ++i) {
            if (!valid_state[i]) continue;
            seq_t val = dp[i];
            if (val == 0) continue;
            int cnt = next_cnt[i];
            for (int k = 0; k < cnt; ++k) {
                int dest = next_state[i][k];
                newdp[dest] += val;
            }
        }

        /* Move to next step. */
        for (int i = 0; i < 1024; ++i) dp[i] = newdp[i];
    }

    /* Sum all ways for the final row pair. */
    seq_t total = 0;
    for (int i = 0; i < 1024; ++i) total += dp[i];
    return total;
}
