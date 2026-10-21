/*
 * A207959  Number of nX7 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of n×7 binary arrays avoiding horizontal 000 and
 *            101, and vertical 001 and 100. Computation by dynamic
 *            programming over 2‑row states with vertical transition groups
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define MAXV 128          /* maximum possible number of valid rows (≤128) */
#define COLMASK 0x7F      /* 7 bits */

/* ------------------------------------------------------------------ */
/* Precomputation data structures                                     */
/* ------------------------------------------------------------------ */
static uint8_t  valid_rows[MAXV];          /* all horizontal‑valid rows */
static int      V;                         /* actual count of valid rows */
static uint8_t  mask0[MAXV];                /* (~row) & 0x7F for each row */
static uint16_t group_start[MAXV][128];    /* start index of each projection
                                           within order array for a given row j */
static uint16_t group_len[MAXV][128];      /* length of each projection group */
static uint16_t order_arr[MAXV][MAXV];     /* concatenated group lists per j */
static bool     precomputed = false;

/* ------------------------------------------------------------------ */
/* Helper: check horizontal validity of a 7‑bit row                   */
/* ------------------------------------------------------------------ */
static bool is_valid_row(uint8_t row)
{
    for (int j = 0; j <= 4; ++j) {
        uint8_t tri = (row >> j) & 0x7;           /* 3 consecutive bits */
        if (tri == 0x0 || tri == 0x5)             /* 000 or 101 forbidden */
            return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Precompute everything once                                         */
/* ------------------------------------------------------------------ */
static void precompute(void)
{
    if (precomputed)
        return;

    /* 1. Enumerate all horizontal‑valid rows (0..127) */
    V = 0;
    for (int r = 0; r < 128; ++r) {
        if (is_valid_row((uint8_t)r)) {
            valid_rows[V++] = (uint8_t)r;
        }
    }

    /* 2. For each row j, compute mask0 and group tables */
    for (int j = 0; j < V; ++j) {
        mask0[j] = (~valid_rows[j]) & COLMASK;

        /* reset counts */
        for (int p = 0; p < 128; ++p)
            group_len[j][p] = 0;

        /* count projection occurrences */
        for (int k = 0; k < V; ++k) {
            uint8_t proj = valid_rows[k] & mask0[j];
            ++group_len[j][proj];
        }

        /* compute start indices (cumulative sums) */
        group_start[j][0] = 0;
        for (int p = 1; p < 128; ++p) {
            group_start[j][p] = group_start[j][p - 1] + group_len[j][p - 1];
        }

        /* temporary counters to fill order_arr */
        uint16_t counter[128] = {0};

        for (int k = 0; k < V; ++k) {
            uint8_t proj = valid_rows[k] & mask0[j];
            uint16_t idx  = group_start[j][proj] + counter[proj];
            order_arr[j][idx] = (uint16_t)k;
            ++counter[proj];
        }
    }

    precomputed = true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    precompute();                                 /* ensure data ready */

    /* Special cases for n = 1 and 2 */
    if (n == 1)
        return (seq_t)V;
    if (n == 2)
        return (seq_t)V * (seq_t)V;

    /* DP arrays: dp_cur and dp_next are [V][V] tables of seq_t */
    static seq_t dp_cur[MAXV][MAXV];
    static seq_t dp_next[MAXV][MAXV];

    /* initialise dp_cur for n = 2 (every pair of rows is allowed) */
    for (int i = 0; i < V; ++i)
        for (int j = 0; j < V; ++j)
            dp_cur[i][j] = (seq_t)1;

    /* Temporary array for sums over projection p (0..127) */
    static seq_t sum_proj[128];

    /* Iterate rows from 3 up to n */
    for (idx_t row = 3; row <= n; ++row) {
        /* Zero dp_next */
        for (int i = 0; i < V; ++i)
            memset(dp_next[i], 0, V * sizeof(seq_t));

        /* For each possible last row j (prev1) */
        for (int j = 0; j < V; ++j) {
            /* Zero sum_proj */
            for (int p = 0; p < 128; ++p)
                sum_proj[p] = 0;

            /* Accumulate sums over all possible prev2 rows i */
            for (int i = 0; i < V; ++i) {
                seq_t val = dp_cur[i][j];
                if (!val)
                    continue;
                uint8_t proj = valid_rows[i] & mask0[j];
                sum_proj[proj] += val;
            }

            /* Distribute contributions to next-state (j,k) */
            for (int p = 0; p < 128; ++p) {
                seq_t add = sum_proj[p];
                if (!add)
                    continue;

                uint16_t start = group_start[j][p];
                uint16_t len   = group_len[j][p];
                for (uint16_t idx = 0; idx < len; ++idx) {
                    uint16_t k = order_arr[j][start + idx];
                    dp_next[j][k] += add;
                }
            }
        }

        /* Swap dp_cur and dp_next for next iteration */
        for (int i = 0; i < V; ++i)
            memcpy(dp_cur[i], dp_next[i], V * sizeof(seq_t));
    }

    /* Sum all entries of dp_cur for the final answer */
    seq_t total = 0;
    for (int i = 0; i < V; ++i)
        for (int j = 0; j < V; ++j)
            total += dp_cur[i][j];

    return total;
}
