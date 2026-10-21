/*
 * A235092  Number of (n+1) X (2+1) 0..5 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 4,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A235092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 16, u64 n <= 35, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A235092: number of (n+1)×3 arrays over {0,…,5} with
   no adjacent equal cells and each 2×2 subblock having its
   diagonal sum differ by exactly 4 from its antidiagonal sum.
   The implementation uses dynamic programming over all valid rows
   (triples with no equal adjacent digits). */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Number of possible values in a cell */
#define VALS          6
/* Total number of 3‑digit base‑6 tuples */
#define MAX_ROWS      (VALS*VALS*VALS)     /* 216 */
/* Maximum number of valid rows after filtering horizontal adjacency */
#define MAX_VALID     MAX_ROWS            /* 216 */

/* ------------------------------------------------------------------ */
/*  Data structures precomputed once                                 */
/* ------------------------------------------------------------------ */
static bool    init_done          = false;
static uint8_t rows_vals[MAX_ROWS][3];

static int compat[MAX_VALID][MAX_VALID];   /* compatibility lists (indices) */
static int compat_cnt[MAX_VALID];          /* number of compatible rows per row */
static size_t valid_cnt;                   /* number of rows that satisfy horizontal constraint */

/* ------------------------------------------------------------------ */
/*  Helper: precompute all valid rows and compatibility relations    */
/* ------------------------------------------------------------------ */
static void init(void)
{
    size_t idx = 0;
    for (uint8_t a = 0; a < VALS; ++a)
        for (uint8_t b = 0; b < VALS; ++b)
            for (uint8_t c = 0; c < VALS; ++c)
            {
                /* horizontal adjacency constraint: a!=b and b!=c */
                if (a == b || b == c)
                    continue;
                rows_vals[idx][0] = a;
                rows_vals[idx][1] = b;
                rows_vals[idx][2] = c;
                ++idx;
            }
    valid_cnt = idx;   /* number of valid rows (should be 150) */

    /* Build compatibility lists between consecutive rows */
    for (size_t i = 0; i < valid_cnt; ++i)
    {
        compat_cnt[i] = 0;
        const uint8_t *ri = rows_vals[i];
        for (size_t j = 0; j < valid_cnt; ++j)
        {
            const uint8_t *rj = rows_vals[j];

            /* vertical adjacency constraint: same column values not equal */
            if (ri[0] == rj[0] || ri[1] == rj[1] || ri[2] == rj[2])
                continue;

            /* 2×2 subblock constraints (columns 0/1 and 1/2) */
            bool good = true;
            for (int col = 0; col < 2; ++col)
            {
                int diag  = ri[col]     + rj[col + 1];
                int anti  = ri[col + 1] + rj[col];
                int diff  = diag - anti;
                if (diff != 4 && diff != -4)
                {
                    good = false;
                    break;
                }
            }
            if (!good)
                continue;

            compat[i][compat_cnt[i]++] = (int)j;
        }
    }
    init_done = true;
}

/* ------------------------------------------------------------------ */
/*  Main sequence evaluation function                                 */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!init_done)
        init();

    /* DP arrays – indexed by valid row index (0 .. valid_cnt-1) */
    static seq_t dp_cur[MAX_VALID];
    static seq_t dp_next[MAX_VALID];

    /* initial state: any valid first row counts as 1 way */
    for (size_t i = 0; i < valid_cnt; ++i)
        dp_cur[i] = 1;

    /* append (n) more rows – now total rows = n+1 */
    for (idx_t step = 1; step <= n; ++step)
    {
        /* reset next-state counters */
        for (size_t i = 0; i < valid_cnt; ++i)
            dp_next[i] = 0;

        /* transition */
        for (size_t i = 0; i < valid_cnt; ++i)
        {
            seq_t val = dp_cur[i];
            if (!val)
                continue;
            for (int k = 0; k < compat_cnt[i]; ++k)
            {
                int j = compat[i][k];
                dp_next[j] += val;
            }
        }

        /* swap dp_cur and dp_next */
        for (size_t i = 0; i < valid_cnt; ++i)
            dp_cur[i] = dp_next[i];
    }

    /* sum over all possible last rows */
    seq_t ans = 0;
    for (size_t i = 0; i < valid_cnt; ++i)
        ans += dp_cur[i];

    return ans;
}
