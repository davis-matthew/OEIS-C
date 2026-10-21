/*
 * A232050  Number of 4Xn 0..1 arrays with no element less than a strict
 *          majority of its horizontal and antidiagonal neighbors.
 * https://oeis.org/A232050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4×n binary arrays with no element less than a
 *            strict majority * of its horizontal and antidiagonal neighbours.
 *            * This implementation builds a validity table for all column
 *            configurations * and then performs dynamic programming over
 *            columns
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#define NO 16

static bool built = false;
static bool valid_[NO + 1][16][NO + 1];
static unsigned bits_[NO][4];

static void build_valid(void)
{
    if (built) return;
    built = true;

    /* Pre‑compute bits for each 4‑row column value. */
    for (int s = 0; s < NO; ++s)
        for (int r = 0; r < 4; ++r)
            bits_[s][r] = (s >> r) & 1U;

    for (int l = 0; l <= NO; ++l)
        for (int m = 0; m < 16; ++m)
            for (int r = 0; r <= NO; ++r)
            {
                bool ok = true;
                for (int row = 0; row < 4 && ok; ++row)
                {
                    int neigh = 0, ones = 0;

                    /* Left neighbour column (if exists). */
                    if (l != NO)
                    {
                        if (bits_[l][row]) ++ones;
                        ++neigh;                   /* left */
                        if (row < 3)
                        {
                            if (bits_[l][row + 1]) ++ones;
                            ++neigh;               /* down‑left */
                        }
                    }

                    /* Right neighbour column (if exists). */
                    if (r != NO)
                    {
                        if (bits_[r][row]) ++ones;
                        ++neigh;                   /* right */
                        if (row > 0)
                        {
                            if (bits_[r][row - 1]) ++ones;
                            ++neigh;               /* up‑right */
                        }
                    }

                    /* Enforce the rule: if a strict majority of neighbours are 1,
                     * the cell itself must be 1. */
                    if (neigh > 0 && 2 * ones > neigh && bits_[m][row] == 0U)
                        ok = false;
                }
                valid_[l][m][r] = ok;
            }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 16;  /* 4×1 arrays – all 2⁴ possibilities */

    build_valid();

    /* Dynamic programming tables: dp[prev][curr] holds number of arrays of
     * length "col" ending with columns prev and curr. */
    seq_t dp[16][16];
    seq_t dp_next[16][16];

    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            dp[i][j] = (seq_t)0;

    /* Initialise with the first two columns. */
    for (int c0 = 0; c0 < 16; ++c0)
        for (int c1 = 0; c1 < 16; ++c1)
            if (valid_[NO][c0][c1])
                dp[c0][c1] = (seq_t)1;

    /* Add columns 2 … n-1. */
    for (idx_t col = 2; col < n; ++col)
    {
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp_next[i][j] = (seq_t)0;

        for (int prev = 0; prev < 16; ++prev)
            for (int curr = 0; curr < 16; ++curr)
            {
                seq_t ways = dp[prev][curr];
                if (ways == 0) continue;

                for (int nxt = 0; nxt < 16; ++nxt)
                    if (valid_[prev][curr][nxt])
                        dp_next[curr][nxt] += ways;
            }

        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = dp_next[i][j];
    }

    /* Sum arrays whose last column is valid with no right neighbour. */
    seq_t result = (seq_t)0;
    for (int prev = 0; prev < 16; ++prev)
        for (int curr = 0; curr < 16; ++curr)
            if (valid_[prev][curr][NO])
                result += dp[prev][curr];

    return result;
}
