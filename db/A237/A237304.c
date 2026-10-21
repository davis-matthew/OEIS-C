/*
 * A237304  Number of (n+1)X(4+1) 0..2 arrays with the upper median of every
 *          2X2 subblock equal.
 * https://oeis.org/A237304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A237304: Number of (n+1)×5 0..2 arrays whose 2×2 subblocks all have the same
 * upper median.  The function uses a transfer‑matrix dynamic programme:
 *   • Each 5‑tuple row is a state (3^5 = 243 states).
 *   • Two states a,b are adjacent under median M∈{0,1,2} iff every
 *     2×2 block formed by a and b has upper median M.
 *   • For each M we count walks of length n (n transitions = n+1 rows)
 *     in the directed graph defined by the adjacency.  The sum over M
 *     gives a(n).
 * The algorithm runs in O(n·states²) with a tiny constant and works
 * for arbitrary‑precision seq_t; for 64‑bit builds it is correct up
 * to the largest n whose value fits in 64 bits.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

enum { STATES = 243 };                /* 3^5 */

/* Base‑3 digit representations of all states. */
static unsigned char digits[STATES][5];

/* For each median M and state a, list of compatible next states. */
static unsigned int adj[3][STATES][STATES];
static unsigned int deg[3][STATES];
static bool initialized = false;

/* Build digit table for all states. */
static void build_digits(void)
{
    for (int s = 0; s < STATES; ++s) {
        int tmp = s;
        for (int i = 0; i < 5; ++i) {
            digits[s][i] = (unsigned char)(tmp % 3);
            tmp /= 3;
        }
    }
}

/* Build adjacency lists for all medians. */
static void build_adjacency(void)
{
    for (int M = 0; M <= 2; ++M) {
        for (int a = 0; a < STATES; ++a) {
            unsigned int d = 0;
            for (int b = 0; b < STATES; ++b) {
                bool ok = true;
                for (int j = 0; j < 4 && ok; ++j) {
                    int v0 = digits[a][j];
                    int v1 = digits[a][j + 1];
                    int v2 = digits[b][j];
                    int v3 = digits[b][j + 1];
                    int vals[4] = { v0, v1, v2, v3 };
                    int le = 0, ge = 0;
                    for (int k = 0; k < 4; ++k) {
                        if (vals[k] <= M) ++le;
                        if (vals[k] >= M) ++ge;
                    }
                    if (le < 3 || ge < 2) ok = false;
                }
                if (ok) {
                    adj[M][a][d++] = b;
                }
            }
            deg[M][a] = d;
        }
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        build_digits();
        build_adjacency();
        initialized = true;
    }

    seq_t total = 0;
    seq_t dp[STATES];
    seq_t dp_next[STATES];

    for (int M = 0; M <= 2; ++M) {
        /* Start with any first row. */
        for (int s = 0; s < STATES; ++s) dp[s] = 1;

        /* Perform n transitions (yielding n+1 rows). */
        for (idx_t step = 0; step < n; ++step) {
            /* Clear next dp. */
            for (int s = 0; s < STATES; ++s) dp_next[s] = 0;

            /* Transition from each current state. */
            for (int cur = 0; cur < STATES; ++cur) {
                seq_t val = dp[cur];
                if (val == 0) continue;
                unsigned int d = deg[M][cur];
                for (unsigned int k = 0; k < d; ++k) {
                    int nxt = adj[M][cur][k];
                    dp_next[nxt] += val;
                }
            }

            /* Prepare for next step. */
            for (int s = 0; s < STATES; ++s) dp[s] = dp_next[s];
        }

        /* Sum all ending states. */
        for (int s = 0; s < STATES; ++s) total += dp[s];
    }

    return total;
}
