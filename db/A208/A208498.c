/*
 * A208498  Number of nX5 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 1 0 and 1 1 1 vertically.
 * https://oeis.org/A208498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays avoiding * horizontally 000
 *            and 001, and vertically 010 and 111
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 22, u64 n <= 48, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* 5‑bit rows that avoid 000 and 001 horizontally. */
static int   valid_rows[32];
static int   valid_count = 0;
static int   row_index[32];

/* For each pair of previous rows, list all admissible next rows. */
static int   next_rows[256][16];
static int   next_cnt[256];

/* DP tables for counting arrays of a given height. */
static seq_t dp[256];
static seq_t ndp[256];

/* One‑time initialization of the row set and transition tables. */
static void init_once(void)
{
    /* Map every 5‑bit pattern to its index or -1. */
    for (int r = 0; r < 32; ++r) row_index[r] = -1;

    /* Identify valid rows. */
    for (int r = 0; r < 32; ++r) {
        bool ok = true;
        for (int pos = 0; pos <= 2; ++pos) {
            int sub = (r >> pos) & 7;              /* 3‑bit window */
            if (sub == 0 || sub == 1) {            /* 000 or 001 */
                ok = false;
                break;
            }
        }
        if (ok) {
            valid_rows[valid_count] = r;
            row_index[r] = valid_count;
            ++valid_count;
        }
    }

    /* valid_count shall be 16 for width 5. */
    for (int pair = 0; pair < 256; ++pair) next_cnt[pair] = 0;

    /* Compute transitions: for each pair (prev2, prev1) keep all next rows. */
    for (int i = 0; i < valid_count; ++i) {
        for (int j = 0; j < valid_count; ++j) {
            int pair = i * valid_count + j;      /* 0..255 */
            for (int k = 0; k < valid_count; ++k) {
                bool ok = true;
                int r1 = valid_rows[i];
                int r2 = valid_rows[j];
                int r3 = valid_rows[k];
                for (int col = 0; col < 5 && ok; ++col) {
                    int b1 = (r1 >> col) & 1;
                    int b2 = (r2 >> col) & 1;
                    int b3 = (r3 >> col) & 1;
                    int triple = (b1 << 2) | (b2 << 1) | b3;
                    if (triple == 0b010 || triple == 0b111) ok = false;
                }
                if (ok) {
                    int idx = next_cnt[pair];
                    next_rows[pair][idx] = k;
                    ++next_cnt[pair];
                }
            }
        }
    }
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_once();
        initialized = true;
    }

    /* Base cases. */
    if (n == 0) return (seq_t)0;                    /* defined only for n ≥ 1 */
    if (n == 1) return (seq_t)valid_count;          /* 16 */
    if (n == 2) return (seq_t)valid_count * (seq_t)valid_count;  /* 256 */

    /* DP over pairs of rows. */
    for (int i = 0; i < 256; ++i) dp[i] = (seq_t)0;
    /* All pairs of first two rows are allowed. */
    for (int i = 0; i < valid_count; ++i)
        for (int j = 0; j < valid_count; ++j)
            dp[i * valid_count + j] = (seq_t)1;

    /* Extend the matrix height from 3 up to n. */
    for (idx_t step = 3; step <= n; ++step) {
        for (int i = 0; i < 256; ++i) ndp[i] = (seq_t)0;
        for (int pair = 0; pair < 256; ++pair) {
            seq_t val = dp[pair];
            if (val == (seq_t)0) continue;
            int i = pair / valid_count;
            int j = pair % valid_count;
            for (int t = 0; t < next_cnt[pair]; ++t) {
                int k = next_rows[pair][t];
                int new_pair = j * valid_count + k;
                ndp[new_pair] += val;
            }
        }
        /* copy ndp back to dp for the next iteration */
        for (int i = 0; i < 256; ++i) dp[i] = ndp[i];
    }

    /* Sum all possible end states. */
    seq_t result = (seq_t)0;
    for (int i = 0; i < 256; ++i) result += dp[i];
    return result;
}
