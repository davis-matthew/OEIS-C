/*
 * A210160  Number of (n+1)X4 0..3 arrays with every 2X2 subblock having two
 *          or three distinct values, and new values 0..3 introduced in row
 *          major order.
 * https://oeis.org/A210160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by dynamic programming over rows, counting *
 *            (n+1)×4 arrays of 0..3 satisfying the 2×2 subblock constraint
 *            and the * RGF property (new values introduced in row‑major
 *            order)
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 20000   /* safety limit larger than 10 000 */

typedef struct {
    uint8_t row;      /* 4‑cell pattern, 2 bits per cell */
    uint8_t max_end;  /* maximum symbol after this row */
} Trans;

/* Transition table: for each previous row (0..255) and start maximum
 * (0..3) store all allowed next rows and resulting maximum. */
static Trans trans_table[256][4][256];
static uint16_t trans_cnt[256][4];

/* Flag indicating that the transition table has been initialized */
static bool trans_initialized = false;

/* DP arrays (previous and next row layers) */
static seq_t dp[256][4];
static seq_t ndp[256][4];

/* Cache for answers: cache[n] = a(n).  cache[0] is unused. */
static seq_t cache[MAX_N + 1];

/* Current number of processed rows (after the first row).  Initially 1. */
static idx_t rows_processed = 1;

/* Highest index for which cache[n] has a valid value (0 means none). */
static idx_t max_n_computed = 0;

/* Flag indicating that the initial row‐state DP has been set up. */
static bool state_initialized = false;

/*-------------------------------------*/
/* Build the transition table once.    */
static void init_transitions(void)
{
    for (int prev = 0; prev < 256; ++prev) {
        for (int ms = 0; ms < 4; ++ms) {
            uint16_t cnt = 0;
            for (int curr = 0; curr < 256; ++curr) {
                /* 2×2 block constraints */
                bool ok = true;
                for (int j = 1; j < 4 && ok; ++j) {
                    int a = (prev >> ((j - 1) * 2)) & 3;
                    int b = (prev >> (j * 2)) & 3;
                    int c = (curr >> ((j - 1) * 2)) & 3;
                    int d = (curr >> (j * 2)) & 3;

                    bool used[4] = {false, false, false, false};
                    used[a] = true;
                    used[b] = true;
                    used[c] = true;
                    used[d] = true;
                    int distinct = 0;
                    for (int v = 0; v < 4; ++v)
                        if (used[v]) ++distinct;

                    if (distinct == 1 || distinct == 4)
                        ok = false;
                }
                if (!ok) continue;

                /* RGF property */
                int curmax = ms;
                bool good = true;
                for (int col = 0; col < 4; ++col) {
                    int color = (curr >> (col * 2)) & 3;
                    if (color > curmax + 1) { good = false; break; }
                    if (color == curmax + 1) curmax = color;
                }
                if (!good) continue;

                trans_table[prev][ms][cnt].row      = (uint8_t)curr;
                trans_table[prev][ms][cnt].max_end  = (uint8_t)curmax;
                ++cnt;
            }
            trans_cnt[prev][ms] = cnt;
        }
    }
}

/* Ensure that the DP state has been advanced up to the requested n and
 * that cache[n] holds the correct result whenever possible. */
static void ensure(idx_t n)
{
    if (n == (idx_t)0) return;

    if (!trans_initialized) {
        init_transitions();
        trans_initialized = true;
    }

    if (!state_initialized) {
        /* Initialise DP after the first row. */
        memset(dp, 0, sizeof(dp));
        memset(ndp, 0, sizeof(ndp));

        for (int curr = 0; curr < 256; ++curr) {
            if ((curr & 3) != 0) continue;          /* first cell must be 0 */
            int curmax = 0;
            bool ok = true;
            for (int col = 0; col < 4; ++col) {
                int color = (curr >> (col * 2)) & 3;
                if (color > curmax + 1) { ok = false; break; }
                if (color == curmax + 1) curmax = color;
            }
            if (!ok) continue;
            dp[curr][curmax] = (seq_t)1;
        }
        rows_processed = 1;
        state_initialized = true;
        max_n_computed = 0;
    }

    /* Extend DP until we have answers up to n */
    while (rows_processed - 1 < n) {
        /* Clear ndp */
        memset(ndp, 0, sizeof(ndp));

        /* Transition from each state */
        for (int prev = 0; prev < 256; ++prev) {
            for (int ms = 0; ms < 4; ++ms) {
                seq_t val = dp[prev][ms];
                if (val == (seq_t)0) continue;

                uint16_t cnt = trans_cnt[prev][ms];
                const Trans *table = trans_table[prev][ms];
                for (uint16_t t = 0; t < cnt; ++t) {
                    const Trans *e = &table[t];
                    ndp[e->row][e->max_end] += val;
                }
            }
        }

        /* Update dp to the next row state */
        memcpy(dp, ndp, sizeof(dp));
        ++rows_processed;

        /* Compute total for this n = rows_processed-1 */
        seq_t total = (seq_t)0;
        for (int r = 0; r < 256; ++r)
            for (int m = 0; m < 4; ++m)
                total += dp[r][m];

        idx_t idx = rows_processed - 1;
        if (idx <= MAX_N) {
            cache[idx] = total;
        }

        if (idx > max_n_computed)
            max_n_computed = idx;
    }
}

/* Public function for OEIS A210160 */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined for n < 1; harmless */
    ensure(n);
    if (n <= MAX_N) return cache[n];

    /* For n > MAX_N we recompute the total from the current DP state. */
    seq_t total = (seq_t)0;
    for (int r = 0; r < 256; ++r)
        for (int m = 0; m < 4; ++m)
            total += dp[r][m];
    return total;
}
