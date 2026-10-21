/*
 * A208700  Number of 5Xn 0..1 arrays avoiding 0 0 1 and 1 0 0 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A208700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A208700: Number of 5×n binary arrays avoiding horizontal
   patterns 0 0 1 and 1 0 0, and vertical patterns 0 1 0 and 1 0 1.
   Computed by dynamic programming over a 16‑state column set.
   Initial values: a(1)=16, a(2)=256. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define HEIGHT 5                 /* fixed number of rows   */
#define COL_MAX 16               /* number of valid columns */

seq_t a(idx_t n)
{
    /* Static state for one‑time initialization */
    static bool initialized = false;
    static uint8_t valid_cols[COL_MAX];
    static bool allowed[COL_MAX][COL_MAX][COL_MAX];
    static seq_t dp_prev[COL_MAX * COL_MAX];
    static seq_t dp_cur[COL_MAX * COL_MAX];
    static seq_t *memo = NULL;       /* memoisation array */
    static size_t memo_len = 0;      /* current number of filled entries (includes zero index) */
    static size_t memo_cap = 0;      /* current capacity of memo array          */
    static idx_t computed_max = 0;   /* highest n for which memo[n] exists      */
    static idx_t col_count = 0;      /* number of valid columns (will be 16)   */

    /* ---------- one‑time initialization ---------- */
    if (!initialized) {
        /* Find all 5‑bit column masks that avoid vertical patterns 0 1 0 and 1 0 1 */
        for (uint8_t mask = 0; mask < (1u << HEIGHT); ++mask) {
            bool ok = true;
            for (int r = 0; r <= HEIGHT - 3; ++r) {
                int b0 = (mask >> r)     & 1;
                int b1 = (mask >> (r+1)) & 1;
                int b2 = (mask >> (r+2)) & 1;
                if ((b0 == 0 && b1 == 1 && b2 == 0) ||
                    (b0 == 1 && b1 == 0 && b2 == 1)) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                valid_cols[col_count++] = mask;
            }
        }
        /* col_count should be 16 */
        /* Precompute horizontal compatibility */
        for (size_t i = 0; i < col_count; ++i) {
            for (size_t j = 0; j < col_count; ++j) {
                for (size_t k = 0; k < col_count; ++k) {
                    bool tr_ok = true;
                    for (size_t r = 0; r < HEIGHT; ++r) {
                        int a = (valid_cols[i] >> r) & 1;
                        int b = (valid_cols[j] >> r) & 1;
                        int c = (valid_cols[k] >> r) & 1;
                        if ((a == 0 && b == 0 && c == 1) ||
                            (a == 1 && b == 0 && c == 0)) {
                            tr_ok = false;
                            break;
                        }
                    }
                    allowed[i][j][k] = tr_ok;
                }
            }
        }
        /* Initialise dp_prev for length 2: every pair of columns is allowed */
        for (size_t i = 0; i < col_count; ++i) {
            for (size_t j = 0; j < col_count; ++j) {
                dp_prev[i * COL_MAX + j] = 1;
            }
        }
        /* Allocate memo array and fill a(1) and a(2) */
        memo_cap = 8;
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        if (!memo) return 0; /* memory failure */
        memo_len = 3;                      /* indices 0..2 used */
        memo[0] = 0;                       /* unused */
        memo[1] = (seq_t)col_count;        /* a(1) */
        memo[2] = (seq_t)col_count * col_count; /* a(2) */
        computed_max = 2;
        initialized = true;
    }

    /* ---------- handle small n ---------- */
    if (n <= computed_max) {
        return memo[(size_t)n];
    }

    /* ---------- extend memoization up to requested n ---------- */
    while (computed_max < n) {
        /* Clear dp_cur for new length */
        for (size_t idx = 0; idx < col_count * col_count; ++idx) {
            dp_cur[idx] = 0;
        }
        /* Transition from dp_prev (length m-1) to dp_cur (length m) */
        for (size_t i = 0; i < col_count; ++i) {
            for (size_t j = 0; j < col_count; ++j) {
                seq_t cur_val = dp_prev[i * COL_MAX + j];
                if (!cur_val) continue;
                for (size_t k = 0; k < col_count; ++k) {
                    if (!allowed[i][j][k]) continue;
                    dp_cur[j * COL_MAX + k] += cur_val;
                }
            }
        }
        /* Move dp_cur into dp_prev for next iteration */
        for (size_t idx = 0; idx < col_count * col_count; ++idx) {
            dp_prev[idx] = dp_cur[idx];
        }
        ++computed_max;  /* we have a(computed_max) now */

        /* Sum all entries in dp_prev to obtain a(computed_max) */
        seq_t total = 0;
        for (size_t idx = 0; idx < col_count * col_count; ++idx) {
            total += dp_prev[idx];
        }

        /* Grow memo array if necessary */
        if (memo_len >= memo_cap) {
            size_t new_cap = memo_cap * 2;
            seq_t *new_mem = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
            if (!new_mem) return 0; /* memory failure */
            memo = new_mem;
            memo_cap = new_cap;
        }
        memo[memo_len++] = total;
    }

    /* Now memo[n] holds the answer */
    return memo[(size_t)n];
}
