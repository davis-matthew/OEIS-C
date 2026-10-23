/*
 * A184192  Half the number of (n+1)X5 binary arrays with no 2X2 subblock
 *          containing exactly one 1.
 * https://oeis.org/A184192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/2 × (the number of (n+1)×5 binary arrays with no 2×2
 *            subblock containing exactly one 1)
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static unsigned char adj[32][32];
static bool adj_ready = false;
static void build_adj(void)
{
    if (adj_ready) return;
    for (int s1 = 0; s1 < 32; ++s1)
    {
        for (int s2 = 0; s2 < 32; ++s2)
        {
            bool ok = true;
            for (int k = 0; k < 4; ++k)
            {
                unsigned b1 = (s1 >> k) & 1;
                unsigned b2 = (s1 >> (k + 1)) & 1;
                unsigned c1 = (s2 >> k) & 1;
                unsigned c2 = (s2 >> (k + 1)) & 1;
                unsigned sum = b1 + b2 + c1 + c2;
                if (sum == 1) { ok = false; break; }
            }
            adj[s1][s2] = ok;
        }
    }
    adj_ready = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; safe guard */
    build_adj();

    const idx_t rows = n + 1;       /* number of rows in the matrix */
    seq_t dp[32];
    seq_t dp_next[32];

    /* initial row: any 5‑bit pattern is allowed, count = 1 */
    for (int i = 0; i < 32; ++i) dp[i] = 1;

    /* iterate through the remaining rows */
    for (idx_t r = 1; r < rows; ++r)
    {
        for (int j = 0; j < 32; ++j) dp_next[j] = 0;
        for (int i = 0; i < 32; ++i)
        {
            seq_t val = dp[i];
            if (val == 0) continue;
            for (int j = 0; j < 32; ++j)
                if (adj[i][j]) dp_next[j] = dp_next[j] + val;
        }
        for (int j = 0; j < 32; ++j) dp[j] = dp_next[j];
    }

    /* total number of admissible arrays of size (n+1)×5 */
    seq_t total = 0;
    for (int i = 0; i < 32; ++i) total = total + dp[i];

    return total / 2;                /* half of the count as required */
}
