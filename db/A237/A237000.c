/*
 * A237000  Number of (n+1) X (1+1) 0..3 arrays with the upper median plus the
 *          lower median of every 2 X 2 subblock equal.
 * https://oeis.org/A237000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count (n+1)×2 arrays over {0,1,2,3} where the sum of the lower *
 *            and upper medians of every 2×2 subblock is the same for all
 *            subblocks. * This implementation uses dynamic programming over
 *            rows for each possible * sum s (0 ≤ s ≤ 6). It works for any
 *            seq_t that supports the usual * integer operators
 * Range:     u8 none, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint8_t adjacency[7][16][16] = {0};
static bool adjacency_initialized = false;

seq_t a(idx_t n)
{
    /* lazily compute adjacency table */
    if (!adjacency_initialized) {
        for (int r1 = 0; r1 < 16; ++r1) {
            for (int r2 = 0; r2 < 16; ++r2) {
                int a = (r1 >> 2) & 3;   /* top‑left */
                int b = r1 & 3;          /* top‑right */
                int c = (r2 >> 2) & 3;   /* bottom‑left */
                int d = r2 & 3;          /* bottom‑right */
                int v[4] = {a, b, c, d};
                /* simple bubble sort of 4 elements */
                for (int i = 0; i < 4; ++i) {
                    for (int j = i + 1; j < 4; ++j) {
                        if (v[i] > v[j]) {
                            int t = v[i];
                            v[i] = v[j];
                            v[j] = t;
                        }
                    }
                }
                int s = v[1] + v[2]; /* lower + upper median (0‑based) */
                if (s >= 0 && s <= 6)
                    adjacency[s][r1][r2] = 1;
            }
        }
        adjacency_initialized = true;
    }

    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    seq_t total = 0;

    for (int s = 0; s <= 6; ++s) {
        seq_t dp[16], nxt[16];

        /* initial row: any of the 16 possible rows */
        for (int i = 0; i < 16; ++i)
            dp[i] = 1;

        /* perform n transitions (n subblocks) */
        for (idx_t step = 1; step <= n; ++step) {
            for (int v = 0; v < 16; ++v)
                nxt[v] = 0;

            for (int u = 0; u < 16; ++u) {
                seq_t cur = dp[u];
                if (!cur)
                    continue;
                const uint8_t *row = adjacency[s][u];
                for (int v = 0; v < 16; ++v)
                    if (row[v])
                        nxt[v] += cur;
            }
            for (int i = 0; i < 16; ++i)
                dp[i] = nxt[i];
        }

        for (int i = 0; i < 16; ++i)
            total += dp[i];
    }

    return total;
}
