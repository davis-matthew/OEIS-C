/*
 * A197437  Number of nX3 0..2 arrays with no element equal to the average of
 *          its horizontal and vertical neighbors.
 * https://oeis.org/A197437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A197437: Number of n×3 arrays with entries 0..2 such that
   no element equals the average of its horizontal and vertical neighbors.
   Implemented using dynamic programming over row states.
   Size of a row: 3 cells, each 0..2 → 3^3 = 27 possible rows. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>          /* for uint32_t */

#define ROWS 27              /* 3^3 possible rows */
#define PAIRS (ROWS*ROWS)    /* number of pairs of consecutive rows */

/* Helper tables */
static unsigned char row_digit[ROWS][3];
static bool valid_top[ROWS][ROWS];
static bool valid_bottom[ROWS][ROWS];
static bool valid_middle[ROWS][ROWS][ROWS];
static uint32_t valid_next_mask[ROWS][ROWS]; /* bitmask of next rows that can follow a pair */

/* Index function for pair (a,b) */
static inline unsigned idx_pair(int a, int b) { return (unsigned)a * ROWS + (unsigned)b; }

/* Initialise helper tables once */
static void init_tables(void)
{
    /* Enumerate all rows */
    for (int val = 0; val < ROWS; ++val) {
        int tmp = val;
        for (int j = 2; j >= 0; --j) {
            row_digit[val][j] = tmp % 3;
            tmp /= 3;
        }
    }

    /* Pre‑compute validity of pairs and triples */
    for (int r0 = 0; r0 < ROWS; ++r0) {
        for (int r1 = 0; r1 < ROWS; ++r1) {
            /* Top row: only below neighbor r1 */
            bool ok = true;
            for (int j = 0; j < 3; ++j) {
                unsigned char v = row_digit[r0][j];
                int cnt = (j>0)+(j<2)+1;                       /* horizontal + below */
                int sum = (j>0?row_digit[r0][j-1]:0)
                         +(j<2?row_digit[r0][j+1]:0)
                         +row_digit[r1][j];
                if ((seq_t)v * cnt == sum) { ok = false; break; }
            }
            valid_top[r0][r1] = ok;

            /* Bottom row: only above neighbor r0 */
            ok = true;
            for (int j = 0; j < 3; ++j) {
                unsigned char v = row_digit[r1][j];
                int cnt = (j>0)+(j<2)+1;                       /* horizontal + above */
                int sum = (j>0?row_digit[r1][j-1]:0)
                         +(j<2?row_digit[r1][j+1]:0)
                         +row_digit[r0][j];
                if ((seq_t)v * cnt == sum) { ok = false; break; }
            }
            valid_bottom[r0][r1] = ok;

            /* Middle rows: both vertical neighbours r0 and r2 */
            for (int r2 = 0; r2 < ROWS; ++r2) {
                ok = true;
                for (int j = 0; j < 3; ++j) {
                    unsigned char v = row_digit[r1][j];
                    int cnt = (j>0)+(j<2)+2;                 /* horizontal + up + down */
                    int sum = (j>0?row_digit[r1][j-1]:0)
                             +(j<2?row_digit[r1][j+1]:0)
                             +row_digit[r0][j]
                             +row_digit[r2][j];
                    if ((seq_t)v * cnt == sum) { ok = false; break; }
                }
                valid_middle[r0][r1][r2] = ok;
                if (ok) valid_next_mask[r0][r1] |= (1U << r2);
            }
        }
    }
}

/* Flag to ensure tables are initialised at most once */
static bool init_done = false;

/* Check if a single row is valid when no vertical neighbours exist */
static bool valid_single_row(int r)
{
    for (int j = 0; j < 3; ++j) {
        unsigned char v = row_digit[r][j];
        int cnt = (j>0)+(j<2);                     /* horizontal only */
        int sum = (j>0?row_digit[r][j-1]:0)
                 +(j<2?row_digit[r][j+1]:0);
        if ((seq_t)v * cnt == sum) return false;
    }
    return true;
}

/* ********** Main sequence function ********** */
seq_t a(idx_t n)
{
    if (!init_done) { init_tables(); init_done = true; }

    /* n == 1 */
    if (n == 1) {
        seq_t count = 0;
        for (int r = 0; r < ROWS; ++r)
            if (valid_single_row(r)) ++count;
        return count;
    }

    /* DP arrays */
    static seq_t dp[PAIRS];
    static seq_t newdp[PAIRS];
    for (int i = 0; i < PAIRS; ++i) dp[i] = 0;

    /* Base: first two rows – check top row validity */
    for (int r0 = 0; r0 < ROWS; ++r0) {
        for (int r1 = 0; r1 < ROWS; ++r1) {
            if (valid_top[r0][r1]) dp[idx_pair(r0,r1)] = 1;
        }
    }

    /* If only two rows, finish */
    if (n == 2) {
        seq_t ans = 0;
        for (int r0 = 0; r0 < ROWS; ++r0) {
            for (int r1 = 0; r1 < ROWS; ++r1) {
                seq_t val = dp[idx_pair(r0,r1)];
                if (val && valid_bottom[r0][r1]) ans += val;
            }
        }
        return ans;
    }

    /* Extend to rows 3 … n */
    for (idx_t step = 2; step < n; ++step) {      /* step: number of rows processed so far */
        for (int i = 0; i < PAIRS; ++i) newdp[i] = 0;

        for (int prevprev = 0; prevprev < ROWS; ++prevprev) {
            for (int prev = 0; prev < ROWS; ++prev) {
                seq_t cur = dp[idx_pair(prevprev,prev)];
                if (!cur) continue;
                uint32_t mask = valid_next_mask[prevprev][prev];
                while (mask) {
                    unsigned next = __builtin_ctz(mask);
                    mask &= mask - 1;
                    newdp[idx_pair(prev,(int)next)] += cur;
                }
            }
        }
        /* copy newdp back into dp for next iteration */
        for (int i = 0; i < PAIRS; ++i) dp[i] = newdp[i];
    }

    /* Final validation of bottom row */
    seq_t ans = 0;
    for (int rprev = 0; rprev < ROWS; ++rprev) {
        for (int rlast = 0; rlast < ROWS; ++rlast) {
            seq_t val = dp[idx_pair(rprev,rlast)];
            if (val && valid_bottom[rprev][rlast]) ans += val;
        }
    }
    return ans;
}
