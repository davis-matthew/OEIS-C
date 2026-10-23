/*
 * A183372  Number of nX5 binary arrays with no element equal to the mod 3 sum
 *          of its diagonal and antidiagonal neighbors.
 * https://oeis.org/A183372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays with no element equal to the
 *            mod 3 sum of its diagonal and anti‑diagonal neighbours, computed
 *            by a transfer–matrix dynamic programming over two rows
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Precompute validity of a triple (prev, curr, next) of 5‑bit rows:
   good[prev][curr][next] is true iff, for every column j,
   curr[j] != (prev[j-1] + prev[j+1] + next[j-1] + next[j+1]) mod 3,
   where out‑of‑range neighbours are ignored. */
static bool good[32][32][32];
static bool good_ready = false;

static void init_good(void)
{
    if (good_ready) return;
    for (int a = 0; a < 32; ++a)
    {
        for (int b = 0; b < 32; ++b)
        {
            for (int c = 0; c < 32; ++c)
            {
                bool ok = true;
                for (int j = 0; j < 5; ++j)
                {
                    int sum = 0;
                    if (j > 0)   sum += (a >> (j - 1)) & 1;
                    if (j < 4)   sum += (a >> (j + 1)) & 1;
                    if (j > 0)   sum += (c >> (j - 1)) & 1;
                    if (j < 4)   sum += (c >> (j + 1)) & 1;
                    sum %= 3;
                    int bit = (b >> j) & 1;
                    if (bit == sum) { ok = false; break; }
                }
                good[a][b][c] = ok;
            }
        }
    }
    good_ready = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    init_good();

    /* dp[prev][curr] holds counts for the current frontier after
       processing some number of rows.  Indices go from 0..31. */
    static seq_t dp[32][32];
    static seq_t dp_next[32][32];

    /* Initialize for the first row.  The dummy previous row is 0. */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = 0;
    for (int p = 0; p < 32; ++p)
        dp[0][p] = 1;

    /* Process rows 2..n.  At each step we add a new row (next). */
    for (idx_t row = 2; row <= n; ++row)
    {
        /* reset dp_next */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp_next[i][j] = 0;

        for (int prev = 0; prev < 32; ++prev)
        {
            for (int curr = 0; curr < 32; ++curr)
            {
                seq_t v = dp[prev][curr];
                if (v == 0) continue;
                for (int next = 0; next < 32; ++next)
                {
                    if (good[prev][curr][next])
                        dp_next[curr][next] += v;
                }
            }
        }

        /* swap dp and dp_next */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = dp_next[i][j];
    }

    /* Sum over all final states, checking the bottom‑row constraint
       (next row is dummy 0). */
    seq_t ans = 0;
    for (int prev = 0; prev < 32; ++prev)
    {
        for (int curr = 0; curr < 32; ++curr)
        {
            if (good[prev][curr][0])
                ans += dp[prev][curr];
        }
    }
    return ans;
}
