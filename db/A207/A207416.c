/*
 * A207416  Number of nX5 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 1 1 and 1 0 1 vertically.
 * https://oeis.org/A207416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 34, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A207416: Number of n×5 binary arrays avoiding
 *  horizontally 001 and 010, and vertically 011 and 101.
 *  The implementation builds the set of 5‑bit rows that
 *  satisfy the horizontal restrictions, precomputes the
 *  allowed transitions between consecutive rows, and
 *  applies a simple DP that walks up to the requested n.
 *  The DP runs in O(n·(row_cnt^2·row_cnt)) time with
 *  a very small constant (~27 × n operations).  The code
 *  works for any seq_t type that is an unsigned integer
 *  (built‑in or arbitrary‑precision) because only
 *  addition, multiplication and comparisons are used.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* 5 columns in each row */
#define COLS 5
/* maximum possible rows (2^5 = 32) */
#define MAX_ROWS 32
/* maximum number of states (row_cnt² ≤ 32² = 1024) */
#define MAX_STATES (MAX_ROWS * MAX_ROWS)

/* cache of valid rows */
static int valid_rows[MAX_ROWS];
static int row_cnt;

/* transition tables */
static int trans_cnt[MAX_STATES];
static int trans_next[MAX_STATES][MAX_ROWS];

/* DP buffers – trimmed to needed capacity */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;
static size_t dp_cap = 0;

/* once‑only construction of rows and transitions */
static void init_once(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* build all 5‑bit rows that avoid 001 and 010 */
    int rc = 0;
    for (int s = 0; s < (1 << COLS); ++s) {
        bool ok = true;
        for (int shift = 0; shift <= COLS - 3 && ok; ++shift) {
            int sub = (s >> shift) & 7;   /* 3‑bit window */
            if (sub == 0b001 || sub == 0b010) ok = false;
        }
        if (ok) valid_rows[rc++] = s;
    }
    row_cnt = rc;                      /* 14 for the sequence */

    /* pre‑compute allowed transitions (i,j) → (j,k) */
    for (int i = 0; i < row_cnt; ++i) {
        for (int j = 0; j < row_cnt; ++j) {
            int state = i * row_cnt + j;
            int trans = 0;
            for (int k = 0; k < row_cnt; ++k) {
                bool good = true;
                for (int col = 0; col < COLS; ++col) {
                    int a = (valid_rows[i] >> (COLS - 1 - col)) & 1;
                    int b = (valid_rows[j] >> (COLS - 1 - col)) & 1;
                    int c = (valid_rows[k] >> (COLS - 1 - col)) & 1;
                    if ((a == 0 && b == 1 && c == 1) ||
                        (a == 1 && b == 0 && c == 1)) {
                        good = false;
                        break;
                    }
                }
                if (good) trans_next[state][trans++] = j * row_cnt + k;
            }
            trans_cnt[state] = trans;
        }
    }
}

/* ensure dp buffers can hold 'states' elements */
static void ensure_dp_cap(size_t states)
{
    if (dp_cap < states) {
        free(dp_cur);
        free(dp_next);
        dp_cur = (seq_t *)malloc(states * sizeof(seq_t));
        dp_next = (seq_t *)malloc(states * sizeof(seq_t));
        dp_cap = states;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined for offset 1, safe */

    init_once();                     /* build tables once */

    /* trivial cases */
    if (n == 1) return (seq_t)row_cnt;

    /* base case n==2 */
    if (n == 2) {
        seq_t v = (seq_t)row_cnt;
        return v * v;
    }

    /* DP for n ≥ 3 */
    size_t states = (size_t)row_cnt * (size_t)row_cnt;
    ensure_dp_cap(states);

    /* initialise dp_cur for length 2: every pair of rows is allowed */
    for (size_t i = 0; i < states; ++i) dp_cur[i] = (seq_t)1;

    /* walk from length 3 up to n */
    for (idx_t len = 3; len <= n; ++len) {
        /* zero the next‑state buffer */
        for (size_t i = 0; i < states; ++i) dp_next[i] = (seq_t)0;

        /* propagate counts */
        for (size_t state = 0; state < states; ++state) {
            seq_t val = dp_cur[state];
            if (!val) continue;
            for (int t = 0; t < trans_cnt[state]; ++t) {
                int next_state = trans_next[state][t];
                dp_next[next_state] += val;
            }
        }
        /* swap buffers for the next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* sum all final states to get total number of arrays */
    seq_t total = (seq_t)0;
    for (size_t i = 0; i < states; ++i) total += dp_cur[i];

    return total;
}
