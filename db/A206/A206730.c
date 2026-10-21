/*
 * A206730  Number of nX4 0..2 arrays avoiding the pattern z-1 z-1 z in any
 *          row, column or nw-to-se diagonal.
 * https://oeis.org/A206730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by dynamic programming over the last two rows * of
 *            a 4×n array over {0,1,2} avoiding the pattern * 0 0 1 or 1 1 2 *
 *            in any row, column or NW‑to‑SE diagonal
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: The code uses seq_t and idx_t as defined by the build system.
 * It does not depend on any external library beyond the standard C headers. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static unsigned char row_digits[81][4];
static int valid_rows[81];
static int val_cnt = 0;
static int row_to_idx[81];       /* maps original row 0..80 to 0..val_cnt-1 or -1 */

/* For DP */
static int total_states;                /* val_cnt * val_cnt */
static int *next_states;                /* flattened arrays of successor state indices */
static size_t *next_counts;             /* number of successors for each state */
static size_t *offset_of_pair;          /* offset of successors in next_states array */

/* Initialization helper */
static void init_rows_and_transitions(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* Compute all 81 possible rows and filter valid ones */
    for (int code = 0; code < 81; ++code) {
        int tmp = code;
        for (int c = 3; c >= 0; --c) {
            row_digits[code][c] = tmp % 3;
            tmp /= 3;
        }
        /* Horizontal check: two windows of length 3 */
        bool ok = true;
        for (int start = 0; start <= 1 && ok; ++start) {
            int a = row_digits[code][start];
            int b = row_digits[code][start + 1];
            int d = row_digits[code][start + 2];
            if (a == b && ((a == 0 && d == 1) || (a == 1 && d == 2)))
                ok = false;
        }
        if (!ok) {
            row_to_idx[code] = -1;
            continue;
        }
        /* Keep valid row */
        row_to_idx[code] = val_cnt;
        valid_rows[val_cnt++] = code;
    }

    total_states = val_cnt * val_cnt;

    /* Allocate arrays for transitions */
    next_states = (int *)malloc(sizeof(int) * total_states * val_cnt); /* max possible */
    next_counts = (size_t *)malloc(sizeof(size_t) * total_states);
    offset_of_pair = (size_t *)malloc(sizeof(size_t) * (total_states + 1));

    size_t off = 0;
    for (int i = 0; i < val_cnt; ++i) {
        for (int j = 0; j < val_cnt; ++j) {
            int pi = valid_rows[i];
            int pj = valid_rows[j];
            int state_idx = i * val_cnt + j;
            size_t cnt = 0;
            size_t state_off = off;
            for (int k = 0; k < val_cnt; ++k) {
                int pk = valid_rows[k];
                /* Check vertical constraints */
                bool bad = false;
                for (int col = 0; col < 4; ++col) {
                    int a = row_digits[pi][col];
                    int b = row_digits[pj][col];
                    int d = row_digits[pk][col];
                    if (a == b) {
                        if (a == 0 && d == 1) { bad = true; break; }
                        if (a == 1 && d == 2) { bad = true; break; }
                    }
                }
                if (bad) continue;
                /* Check diagonal NW-SE constraints */
                for (int col = 0; col <= 1 && !bad; ++col) {
                    int a = row_digits[pi][col];
                    int b = row_digits[pj][col + 1];
                    int d = row_digits[pk][col + 2];
                    if (a == b) {
                        if (a == 0 && d == 1) { bad = true; break; }
                        if (a == 1 && d == 2) { bad = true; break; }
                    }
                }
                if (bad) continue;
                /* Allowed transition */
                int new_state = j * val_cnt + k; /* (prev_row2, new_row) */
                next_states[off++] = new_state;
                ++cnt;
            }
            next_counts[state_idx] = cnt;
            offset_of_pair[state_idx] = state_off;
        }
    }
    offset_of_pair[total_states] = off; /* sentinel */
}

/* DP routine */
seq_t a(idx_t n)
{
    init_rows_and_transitions();

    /* a(1) and a(2) are trivial */
    if (n == 1) {
        return (seq_t)69; /* number of valid rows */
    }
    if (n == 2) {
        seq_t v = 69;
        return v * v;      /* v^2 */
    }

    /* Allocate DP arrays */
    seq_t *dp_curr = (seq_t *)calloc(total_states, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(total_states, sizeof(seq_t));
    if (!dp_curr || !dp_next) return (seq_t)0; /* allocation failed, return 0 */

    /* Initial state at n=2: every pair of valid rows is possible */
    for (int i = 0; i < total_states; ++i) dp_curr[i] = (seq_t)1;

    /* Iterate from row 3 up to n */
    for (idx_t row = 3; row <= n; ++row) {
        /* zero dp_next */
        for (int i = 0; i < total_states; ++i) dp_next[i] = (seq_t)0;

        for (int state = 0; state < total_states; ++state) {
            seq_t val = dp_curr[state];
            if (val == (seq_t)0) continue;

            size_t off = offset_of_pair[state];
            size_t cnt = next_counts[state];
            for (size_t j = 0; j < cnt; ++j) {
                int nxt = next_states[off + j];
                dp_next[nxt] += val;
            }
        }
        /* swap */
        seq_t *tmp = dp_curr;
        dp_curr = dp_next;
        dp_next = tmp;
    }

    /* Sum all states to obtain a(n) */
    seq_t result = (seq_t)0;
    for (int i = 0; i < total_states; ++i) result += dp_curr[i];

    free(dp_curr);
    free(dp_next);
    return result;
}
