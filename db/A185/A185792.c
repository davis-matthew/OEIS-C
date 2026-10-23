/*
 * A185792  Number of (n+1)X4 binary arrays with no 2X2 subblock sum equal to
 *          any horizontal or vertical neighbor 2X2 subblock sum.
 * https://oeis.org/A185792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 20, u64 n <= 45, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements counting of (n+1)×4 binary arrays with
   no equal sums in adjacent horizontal or vertical 2×2 blocks.  */
/* Sequence is A185792.  */
/* The algorithm keeps two consecutive rows as a state.
   For each pair of rows the three horizontal 2×2 sums are
   pre‑computed.  Two rows form a valid pair iff their
   left & middle sums differ and their middle & right sums differ
   (no restriction between the left and right sums).
   A transition from (r₀,r₁) to (r₁,r₂) is allowed iff
   for every column pair j (0..2) the 2×2 sum with rows
   (r₀,r₁) differs from the sum with rows (r₁,r₂).  */

#include <stdint.h>
#include <stdbool.h>

/* auxiliary tables */
static bool horizValid[256];        /* pair (r0,r1) satisfies adjacent horizontal sums differ */
static int  vertSum[256][3];        /* 2×2 sums for each column pair j=0..2 */
static bool tables_ready = false;

/* bit extraction from a 4‑bit row */
static inline int bit(uint8_t row, int c) { return (row >> c) & 1; }

/* precompute 256 tables */
static void init_tables(void)
{
    if (tables_ready) return;
    for (int r0 = 0; r0 < 16; ++r0)
        for (int r1 = 0; r1 < 16; ++r1)
        {
            int idx   = (r0 << 4) | r1;
            int s[3];
            for (int j = 0; j < 3; ++j)
            {
                int sum = bit(r0, j)   + bit(r0, j+1)
                        + bit(r1, j)   + bit(r1, j+1);
                vertSum[idx][j] = sum;
                s[j] = sum;
            }
            /* horizontal constraint: adjacent sums must differ   */
            horizValid[idx] = (s[0] != s[1] && s[1] != s[2]);
        }
    tables_ready = true;
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* n ≥ 1 : grid has (n+1) rows, 4 columns. */
    init_tables();

    /* dp[prev][curr] : number of ways to build rows so far
       ending with last two rows (prev,curr).  */
    seq_t dp[16][16];
    seq_t ndp[16][16];

    /* initialise with the first two rows */
    for (int prev = 0; prev < 16; ++prev)
        for (int curr = 0; curr < 16; ++curr)
            dp[prev][curr] = horizValid[(prev << 4) | curr] ? (seq_t)1 : (seq_t)0;

    /* iterate over remaining rows */
    for (idx_t row = 2; row <= n; ++row)
    {
        /* clear ndp */
        for (int p = 0; p < 16; ++p)
            for (int c = 0; c < 16; ++c)
                ndp[p][c] = (seq_t)0;

        for (int prev = 0; prev < 16; ++prev)
        {
            for (int curr = 0; curr < 16; ++curr)
            {
                seq_t ways = dp[prev][curr];
                if (ways == (seq_t)0) continue;

                int idx_prev_curr = (prev << 4) | curr;

                for (int nxt = 0; nxt < 16; ++nxt)
                {
                    if (!horizValid[(curr << 4) | nxt]) continue;

                    /* vertical constraint: sums for each column pair must differ */
                    bool ok = true;
                    for (int j = 0; j < 3; ++j)
                        if (vertSum[idx_prev_curr][j] == vertSum[(curr << 4) | nxt][j])
                        { ok = false; break; }

                    if (!ok) continue;

                    ndp[curr][nxt] += ways;
                }
            }
        }

        /* move ndp -> dp */
        for (int p = 0; p < 16; ++p)
            for (int c = 0; c < 16; ++c)
                dp[p][c] = ndp[p][c];
    }

    /* sum all final states */
    seq_t total = (seq_t)0;
    for (int prev = 0; prev < 16; ++prev)
        for (int curr = 0; curr < 16; ++curr)
            total += dp[prev][curr];

    return total;
}
