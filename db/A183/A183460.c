/*
 * A183460  Number of (n+4)X7 binary arrays with every 1 having exactly three
 *          king-move neighbors equal to 1 but with no 2X2 blocks of 1s.
 * https://oeis.org/A183460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of (n+4)×7 binary arrays where every 1 has exactly
 *            * three king‑move neighbors equal to 1 and no 2×2 block of 1s. *
 *            The implementation uses a transfer matrix over pairs of
 *            consecutive rows. * States are encoded as (prev_row, curr_row)
 *            pairs. * Precomputation of valid next rows per p...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   no2x2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/no2x2.h"

#define MASKS 128                 // 2^7 possible rows
#define STATES (MASKS * MASKS)    // 16384 possible state pairs

/* True if every 1 in 'cur' has exactly three 1 neighbors among
 * (prev, cur, next).  Relationship between 'prev' and 'cur' must already
 * satisfy no 2×2; this is assumed by the caller but checked for safety. */
static bool row_valid(uint8_t prev, uint8_t cur, uint8_t next)
{
    /* No 2×2 between prev and cur */
    if (!no2x2(prev, cur)) return false;
    for (int j = 0; j < 7; ++j)
    {
        if (!(cur & (1U << j))) continue;     /* only 1 cells matter */

        int cnt = 0;

        if (j > 0) {
            if (prev & (1U << (j - 1))) ++cnt;
            if (cur  & (1U << (j - 1))) ++cnt;
            if (next & (1U << (j - 1))) ++cnt;
        }
        if (prev & (1U << j)) ++cnt;
        if (next & (1U << j)) ++cnt;
        if (j < 6) {
            if (prev & (1U << (j + 1))) ++cnt;
            if (cur  & (1U << (j + 1))) ++cnt;
            if (next & (1U << (j + 1))) ++cnt;
        }

        if (cnt != 3) return false;
    }
    return true;
}

/* Precomputed tables: for each pair (prev, cur) store the list of
 * possible next rows.  Table sizes are small (max 128). */
static uint8_t next_mask[STATES][MASKS];
static uint8_t next_cnt[STATES];
static bool tables_ready = false;

/* Build the transition tables once. */
static void build_tables(void)
{
    if (tables_ready) return;

    for (uint32_t p = 0; p < MASKS; ++p)
    {
        for (uint32_t c = 0; c < MASKS; ++c)
        {
            uint32_t idx = (p << 7) | c;
            if (!no2x2((uint8_t)p, (uint8_t)c))
            {
                next_cnt[idx] = 0;
                continue;
            }

            uint8_t *row = next_mask[idx];
            uint8_t cnt = 0;

            for (uint32_t n = 0; n < MASKS; ++n)
            {
                if (!no2x2((uint8_t)c, (uint8_t)n)) continue;
                if (!row_valid((uint8_t)p, (uint8_t)c, (uint8_t)n)) continue;
                row[cnt++] = (uint8_t)n;
            }
            next_cnt[idx] = cnt;
        }
    }
    tables_ready = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* The sequence has offset 1: a(1) is the first listed term. */
    if (n < 1) return 0;

    build_tables();

    /* DP arrays: dp_cur[state] = count of ways to reach this pair after
     * processing the current number of rows.  State encoded as (prev<<7 | curr). */
    static seq_t dp_cur[STATES];
    static seq_t dp_next[STATES];
    memset(dp_cur, 0, sizeof(dp_cur));

    /* Initial row: dummy row 0 (before first real row) + first real row */
    for (uint32_t m0 = 0; m0 < MASKS; ++m0)
        dp_cur[m0] = 1;   /* state (0, m0) */

    idx_t rows = n + 4;                     /* total number of rows */
    for (idx_t row = 1; row < rows; ++row)  /* we already have 1 row */
    {
        memset(dp_next, 0, sizeof(dp_next));

        for (uint32_t p = 0; p < MASKS; ++p)
        {
            for (uint32_t c = 0; c < MASKS; ++c)
            {
                uint32_t state = (p << 7) | c;
                seq_t ways = dp_cur[state];
                if (ways == 0) continue;

                uint8_t *nxt = next_mask[state];
                uint8_t cnt = next_cnt[state];
                for (uint8_t k = 0; k < cnt; ++k)
                {
                    uint8_t n = nxt[k];
                    uint32_t next_state = (c << 7) | n;
                    dp_next[next_state] += ways;
                }
            }
        }
        /* swap dp arrays for next iteration */
        memcpy(dp_cur, dp_next, sizeof(dp_cur));
    }

    /* After all rows processed, validate the last row against zero bottom row. */
    seq_t ans = 0;
    for (uint32_t p = 0; p < MASKS; ++p)
    {
        for (uint32_t c = 0; c < MASKS; ++c)
        {
            uint32_t state = (p << 7) | c;
            seq_t ways = dp_cur[state];
            if (ways == 0) continue;
            if (!row_valid((uint8_t)p, (uint8_t)c, 0)) continue;
            ans += ways;
        }
    }
    return ans;
}
