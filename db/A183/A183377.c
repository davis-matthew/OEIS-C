/*
 * A183377  Number of nX4 binary arrays with no element equal to the mod 3 sum
 *          of its king-move neighbors.
 * https://oeis.org/A183377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary matrices with no element equal to
 *            the mod 3 sum of its king‑move neighbors
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 36, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static bool tbl_valid[16][16][16];
static bool tbl_ready = false;

static void init_tbl(void)
{
    if (tbl_ready)
        return;
    tbl_ready = true;
    for (int prev = 0; prev < 16; ++prev)
    {
        for (int curr = 0; curr < 16; ++curr)
        {
            for (int nxt = 0; nxt < 16; ++nxt)
            {
                bool ok = true;
                for (int col = 0; col < 4 && ok; ++col)
                {
                    int sum = 0;
                    for (int dr = -1; dr <= 1; ++dr)
                    {
                        int row_bits = (dr == -1) ? prev :
                                       (dr == 0) ? curr : nxt;
                        for (int dc = -1; dc <= 1; ++dc)
                        {
                            if (dr == 0 && dc == 0)
                                continue;
                            int ncol = col + dc;
                            if (ncol < 0 || ncol >= 4)
                                continue;
                            sum += (row_bits >> ncol) & 1;
                        }
                    }
                    int mod = sum % 3;
                    int bit = (curr >> col) & 1;
                    if (bit == mod)
                        ok = false;
                }
                tbl_valid[prev][curr][nxt] = ok;
            }
        }
    }
}

seq_t a(idx_t n)
{
    init_tbl();

    /* dp[prev][curr] = number of ways for the processed rows */
    seq_t dp[16][16];
    seq_t nxt[16][16];

    /* initialise for the first real row (row 1).  The virtual row 0 is all zeros. */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            dp[i][j] = 0;
    for (int first = 0; first < 16; ++first)
        dp[0][first] = 1;

    /* process rows 2 … n */
    for (idx_t r = 1; r < n; ++r)
    {
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                nxt[i][j] = 0;

        for (int p = 0; p < 16; ++p)
        {
            for (int q = 0; q < 16; ++q)
            {
                seq_t ways = dp[p][q];
                if (!ways)
                    continue;
                for (int nr = 0; nr < 16; ++nr)
                {
                    if (!tbl_valid[p][q][nr])
                        continue;
                    nxt[q][nr] += ways;
                }
            }
        }

        /* swap dp and nxt */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = nxt[i][j];
    }

    /* final validation: last row with a virtual zero row below */
    seq_t result = 0;
    for (int p = 0; p < 16; ++p)
    {
        for (int q = 0; q < 16; ++q)
        {
            if (!dp[p][q])
                continue;
            if (!tbl_valid[p][q][0])
                continue;
            result += dp[p][q];
        }
    }
    return result;
}
