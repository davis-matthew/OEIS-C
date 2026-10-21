/*
 * A233719  Number of (n+1) X (2+1) 0..7 arrays with every 2 X 2 subblock
 *          having the sum of the squares of the edge differences equal to 26.
 * https://oeis.org/A233719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A233719:  a(n) is the number of (n+1)×3 arrays with entries 0..7 such that
   every 2×2 subblock has the sum of the squares of the edge differences
   equal to 26.
   Computed by dynamic programming over the 512 possible 3‑column states. */
#include <stdint.h>
#include <stdbool.h>

/* 512 possible states (a0,a1,a2) with 0<=ai<=7, encoded as a0*64+a1*8+a2. */
static uint16_t trans_next[512][512];
static uint8_t  trans_cnt[512];
static bool     initialized = false;

static void init_graph(void)
{
    if (initialized) return;
    for (int s = 0; s < 512; ++s)
    {
        unsigned char a0 = s >> 6;
        unsigned char a1 = (s >> 3) & 7;
        unsigned char a2 = s & 7;

        for (int t = 0; t < 512; ++t)
        {
            unsigned char b0 = t >> 6;
            unsigned char b1 = (t >> 3) & 7;
            unsigned char b2 = t & 7;

            int da0a1 = a0 - a1;
            int dab0  = a0 - b0;
            int dab1  = a1 - b1;
            int db0b1 = b0 - b1;
            int sum0  = da0a1*da0a1 + dab0*dab0 + dab1*dab1 + db0b1*db0b1;

            int da1a2 = a1 - a2;
            int db1b2 = b1 - b2;
            int sum1  = da1a2*da1a2 + dab1*dab1 + (a2 - b2)*(a2 - b2) + db1b2*db1b2;

            if (sum0 == 26 && sum1 == 26)
            {
                uint8_t c = trans_cnt[s];
                trans_next[s][c] = (uint16_t)t;
                trans_cnt[s] = c + 1;
            }
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    init_graph();

    seq_t dp[512];
    seq_t next[512];

    for (int i = 0; i < 512; ++i) dp[i] = (seq_t)1;   /* any first column is allowed */

    for (idx_t step = 0; step < n; ++step)
    {
        for (int i = 0; i < 512; ++i) next[i] = (seq_t)0;

        for (int i = 0; i < 512; ++i)
        {
            seq_t val = dp[i];
            if (!val) continue;
            uint8_t cnt = trans_cnt[i];
            for (uint8_t k = 0; k < cnt; ++k)
            {
                int j = trans_next[i][k];
                next[j] += val;
            }
        }

        for (int i = 0; i < 512; ++i) dp[i] = next[i];
    }

    seq_t total = 0;
    for (int i = 0; i < 512; ++i) total += dp[i];
    return total;
}
