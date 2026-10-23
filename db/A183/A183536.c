/*
 * A183536  Number of nX2 0..3 arrays with each element equal to either the
 *          sum mod 4 of its horizontal and vertical neighbors or the sum mod
 *          4 of its diagonal and antidiagonal neighbors.
 * https://oeis.org/A183536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by dynamic programming over rows of the n×2
 *            grid. Each row is encoded as 4 bits (two 2‑bit values).
 *            Transition: a(1)=4, a(2)=16, a(3)=43, … . The recurrence is not
 *            a simple closed form, so we use the DP described below
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Encode a row with two 2‑bit values: bits 0‑1 = column 0, bits 2‑3 = column 1. */
static inline uint16_t encode_row(uint8_t c0, uint8_t c1)
{
    return (c0 & 3) | ((c1 & 3) << 2);
}

/* Test whether a row satisfies its constraints when the rows above
   and below are given (a zero row represents a missing neighbour). */
static inline bool row_satisfied(uint16_t prev, uint16_t curr, uint16_t next)
{
    uint8_t a0 = curr & 3;
    uint8_t a1 = (curr >> 2) & 3;

    uint8_t p0 = prev & 3;
    uint8_t p1 = (prev >> 2) & 3;

    uint8_t n0 = next & 3;
    uint8_t n1 = (next >> 2) & 3;

    /* Cell at column 0 */
    uint8_t horiz0 = a1;                  /* horizontal neighbour */
    uint8_t vert0   = p0 + n0;            /* vertical neighbours (missing count as 0) */
    uint8_t sum0    = (horiz0 + vert0) % 4;
    uint8_t diag0   = (p1 + n1) % 4;      /* diagonal neighbours */
    if (a0 != sum0 && a0 != diag0)
        return false;

    /* Cell at column 1 */
    uint8_t horiz1 = a0;
    uint8_t vert1   = p1 + n1;
    uint8_t sum1    = (horiz1 + vert1) % 4;
    uint8_t diag1   = (p0 + n0) % 4;
    if (a1 != sum1 && a1 != diag1)
        return false;

    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined but harmless */

    const uint16_t zero_row = 0;       /* row with both cells 0 */

    /* Special case n == 1 */
    if (n == 1) {
        seq_t tot = 0;
        for (uint16_t r = 0; r < 16; ++r)
            if (row_satisfied(zero_row, r, zero_row))
                ++tot;
        return tot;
    }

    /* DP tables: dp[prev][curr] = number of ways up to current row
       where the last two rows are prev and curr (all earlier rows are valid). */
    seq_t dp[16][16];
    seq_t newdp[16][16];

    /* Initialise dp for the first two rows */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            dp[i][j] = 0;

    for (uint16_t r1 = 0; r1 < 16; ++r1) {
        for (uint16_t r2 = 0; r2 < 16; ++r2) {
            if (row_satisfied(zero_row, r1, r2))
                ++dp[r1][r2];
        }
    }

    /* Process rows 3 .. n */
    for (idx_t row = 3; row <= n; ++row) {
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                newdp[i][j] = 0;

        for (uint16_t prev = 0; prev < 16; ++prev) {
            for (uint16_t curr = 0; curr < 16; ++curr) {
                seq_t cnt = dp[prev][curr];
                if (!cnt)
                    continue;
                for (uint16_t next = 0; next < 16; ++next) {
                    if (row_satisfied(prev, curr, next))
                        newdp[curr][next] += cnt;
                }
            }
        }

        /* copy newdp to dp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* Finalise: validate the last row */
    seq_t total = 0;
    for (uint16_t prev = 0; prev < 16; ++prev) {
        for (uint16_t last = 0; last < 16; ++last) {
            seq_t cnt = dp[prev][last];
            if (!cnt)
                continue;
            if (row_satisfied(prev, last, zero_row))
                total += cnt;
        }
    }
    return total;
}
