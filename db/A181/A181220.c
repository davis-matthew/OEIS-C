/*
 * A181220  Number of n X 4 binary matrices with no three 1's adjacent in a
 *          line horizontally, vertically, diagonally or antidiagonally.
 * https://oeis.org/A181220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n × 4 binary matrices with no three 1's
 *            adjacent horizontally, vertically, diagonally or antidiagonally.
 *            Computed by dynamic programming over pairs of consecutive rows
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Helper macro to test a bit in a 4‑bit integer */
#define BIT(row, j) (((row) >> (j)) & 1ULL)

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, but safe */
    if (n == 1) return (seq_t)13;         /* 4‑bit rows with no 111 pattern */

    /* Precompute which 4‑bit rows are horizontally valid */
    static const bool valid_row[16] = {
        /* 0b0000 */ true,
        /* 0b0001 */ true,
        /* 0b0010 */ true,
        /* 0b0011 */ true,
        /* 0b0100 */ true,
        /* 0b0101 */ true,
        /* 0b0110 */ true,
        /* 0b0111 */ false,  /* contains 111 */
        /* 0b1000 */ true,
        /* 0b1001 */ true,
        /* 0b1010 */ true,
        /* 0b1011 */ true,
        /* 0b1100 */ true,
        /* 0b1101 */ true,
        /* 0b1110 */ false,  /* contains 111 */
        /* 0b1111 */ false   /* contains 1111 */
    };

    /* DP arrays: dp[prevprev*16 + prev] holds count of ways for the last
       two rows being (prevprev, prev). 256 states max. */
    static seq_t dp[256];
    static seq_t newdp[256];

    /* Initialize for the first row; use dummy prevprev = 0 */
    for (size_t i = 0; i < 256; ++i) dp[i] = 0;
    for (int r = 0; r < 16; ++r)
        if (valid_row[r]) dp[0 * 16 + r] = 1;

    /* Iterate over rows 2 .. n */
    for (idx_t row = 2; row <= n; ++row)
    {
        for (size_t i = 0; i < 256; ++i) newdp[i] = 0;

        for (size_t idx = 0; idx < 256; ++idx)
        {
            seq_t count = dp[idx];
            if (count == 0) continue;

            int prevprev = (int)(idx >> 4);   /* idx / 16 */
            int prev     = (int)(idx & 0xF);  /* idx % 16  */

            /* Try each possible current row */
            for (int cur = 0; cur < 16; ++cur)
            {
                if (!valid_row[cur]) continue;

                /* Vertical triple check */
                bool bad = false;
                for (int j = 0; j < 4; ++j)
                    if (BIT(prevprev, j) && BIT(prev, j) && BIT(cur, j))
                    { bad = true; break; }
                if (bad) continue;

                /* Main diagonal (slope +1) triple check */
                for (int j = 2; j < 4; ++j)
                    if (BIT(prevprev, j-2) && BIT(prev, j-1) && BIT(cur, j))
                    { bad = true; break; }
                if (bad) continue;

                /* Anti-diagonal (slope -1) triple check */
                for (int j = 0; j < 2; ++j)
                    if (BIT(prevprev, j+2) && BIT(prev, j+1) && BIT(cur, j))
                    { bad = true; break; }
                if (bad) continue;

                size_t nidx = (size_t)prev * 16 + (size_t)cur;
                newdp[nidx] += count;
            }
        }

        /* Move newdp to dp for next iteration */
        for (size_t i = 0; i < 256; ++i) dp[i] = newdp[i];
    }

    /* Sum over all states after processing n rows */
    seq_t total = 0;
    for (size_t i = 0; i < 256; ++i) total += dp[i];

    return total;
}
