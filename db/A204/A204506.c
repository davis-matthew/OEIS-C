/*
 * A204506  Number of (n+2)X3 0..2 arrays with every 3X3 subblock having three
 *          equal elements in a row horizontally, vertically, diagonally or
 *          antidiagonally at least two different ways, and new values 0..2
 *          introduced in row major order.
 * https://oeis.org/A204506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×3 arrays over {0,1,2} whose every * 3×3
 *            subblock contains at least two equal‑element lines (horizontal,
 *            * vertical, main or anti‑diagonal) and where the three symbols
 *            are * introduced in row‑major order. The sequence is computed by
 *            dynamic * programming over rows. The table...
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 16, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Constants for the 3×3 grid */
#define ROWS          27            /* 3^3 possible rows */
#define STATE_COUNT   (ROWS * ROWS) /* 729, previous two rows */
#define MASK_COUNT    8             /* 3 bits → 0..7 */

/* Pre‑computed tables */
static int  row_digits[ROWS][3];      /* digits of each row */
static int  row_mask   [ROWS];        /* bitmask of digits present in the row */
static int  first_idx  [ROWS][3];     /* first column index of each digit (-1 if absent) */

static int  next_rows  [STATE_COUNT][ROWS];   /* for each state: list of r3 that satisfy the subblock rule */
static unsigned char next_count[STATE_COUNT];

/* Helper to generate subblock validity lists */
static bool valid_triple(int r1, int r2, int r3)
{
    int cnt = 0;

    /* Sub‑horizontal lines */
    for (int r = 0; r < 3; ++r) {
        int d0, d1, d2;
        if (r == 0) { d0 = row_digits[r1][0]; d1 = row_digits[r1][1]; d2 = row_digits[r1][2]; }
        else if (r == 1) { d0 = row_digits[r2][0]; d1 = row_digits[r2][1]; d2 = row_digits[r2][2]; }
        else { d0 = row_digits[r3][0]; d1 = row_digits[r3][1]; d2 = row_digits[r3][2]; }
        if (d0 == d1 && d1 == d2) ++cnt;
    }

    /* Sub‑vertical lines */
    for (int c = 0; c < 3; ++c) {
        int a = row_digits[r1][c];
        int b = row_digits[r2][c];
        int c3 = row_digits[r3][c];
        if (a == b && b == c3) ++cnt;
    }

    /* Main diagonal */
    if (row_digits[r1][0] == row_digits[r2][1] &&
        row_digits[r2][1] == row_digits[r3][2]) ++cnt;

    /* Anti‑diagonal */
    if (row_digits[r1][2] == row_digits[r2][1] &&
        row_digits[r2][1] == row_digits[r3][0]) ++cnt;

    return cnt >= 2;
}

/* Initialise lookup tables once */
static void init_tables(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* Build row representation, masks and first indices */
    for (int i = 0; i < ROWS; ++i) {
        int d0 = (i / 9) % 3;
        int d1 = (i / 3) % 3;
        int d2 = i % 3;
        row_digits[i][0] = d0;
        row_digits[i][1] = d1;
        row_digits[i][2] = d2;

        int mask = 0;
        int first[3] = { -1, -1, -1 };
        for (int c = 0; c < 3; ++c) {
            int d = row_digits[i][c];
            mask |= 1 << d;
            if (first[d] == -1) first[d] = c;
        }
        row_mask[i] = mask;
        first_idx[i][0] = first[0];
        first_idx[i][1] = first[1];
        first_idx[i][2] = first[2];
    }

    /* Build transition lists based solely on subblock rule */
    for (int r1 = 0; r1 < ROWS; ++r1) {
        for (int r2 = 0; r2 < ROWS; ++r2) {
            int state = r1 * ROWS + r2;
            unsigned char c = 0;
            for (int r3 = 0; r3 < ROWS; ++r3) {
                if (valid_triple(r1, r2, r3)) {
                    next_rows[state][c++] = r3;
                }
            }
            next_count[state] = c;
        }
    }
}

/* Apply canonical‑labeling constraint to a single row.
 * Returns new mask if the row is valid, or -1 otherwise. */
static int new_mask_from_row(int r, int mask_before)
{
    int new_bits = row_mask[r] & ~mask_before; /* bits that are new in this row */

    if (new_bits == 0) {
        /* No new symbols */
        return mask_before | row_mask[r];
    }

    /* Count new bits */
    int pop = 0;
    int first_new = -1;
    int second_new = -1;
    for (int d = 0; d < 3; ++d) {
        if (new_bits & (1 << d)) {
            ++pop;
            if (first_new == -1) first_new = d;
            else second_new = d;
        }
    }

    if (pop == 1) {
        /* Only one new value; it must be the smallest unused */
        int smallest = 0;
        for (int d = 0; d < 3; ++d) {
            if (!(mask_before & (1 << d))) { smallest = d; break; }
        }
        if (first_new != smallest) return -1;
        return mask_before | (1 << first_new);
    }

    if (pop == 2) {
        /* Two new values: must be 1 and 2 with 1 appearing before 2 */
        if (!(new_bits & (1 << 1)) || !(new_bits & (1 << 2))) return -1;
        if (first_idx[r][1] > first_idx[r][2]) return -1;
        /* smallest unused must be 1 */
        int smallest = 0;
        for (int d = 0; d < 3; ++d) {
            if (!(mask_before & (1 << d))) { smallest = d; break; }
        }
        if (smallest != 1) return -1;
        return mask_before | (1 << 1) | (1 << 2);
    }

    /* No other case possible */
    return -1;
}

/* The sequence function */
seq_t a(idx_t n)
{
    /* a(0) is unspecified in the sequence */
    if (n == 0) return 0;

    init_tables();

    const idx_t total_rows = n + 2;
    const idx_t transitions = total_rows - 2;        /* number of rows after the first two */

    static seq_t dp_cur[STATE_COUNT * MASK_COUNT];
    static seq_t dp_next[STATE_COUNT * MASK_COUNT];

    const size_t dim = STATE_COUNT * MASK_COUNT;
    memset(dp_cur, 0, sizeof(dp_cur));
    memset(dp_next, 0, sizeof(dp_next));

    /* --- Initialise with the first two rows --- */
    for (int r1 = 0; r1 < ROWS; ++r1) {
        /* First row must start with 0 */
        if (row_digits[r1][0] != 0) continue;

        int mask1 = new_mask_from_row(r1, 1);
        if (mask1 == -1) continue;   /* should not happen for r1[0]==0 */

        for (int r2 = 0; r2 < ROWS; ++r2) {
            int mask2 = new_mask_from_row(r2, mask1);
            if (mask2 == -1) continue;
            int state = r1 * ROWS + r2;
            size_t idx = (size_t)state * MASK_COUNT + mask2;
            dp_cur[idx] += 1;
        }
    }

    /* --- Process remaining rows --- */
    for (idx_t step = 0; step < transitions; ++step) {
        memset(dp_next, 0, sizeof(dp_next));

        for (int state = 0; state < STATE_COUNT; ++state) {
            int r1 = state / ROWS;
            int r2 = state % ROWS;
            for (int mask = 0; mask < MASK_COUNT; ++mask) {
                size_t idx = (size_t)state * MASK_COUNT + mask;
                seq_t cur_val = dp_cur[idx];
                if (cur_val == 0) continue;

                for (unsigned char k = 0; k < next_count[state]; ++k) {
                    int r3 = next_rows[state][k];
                    int new_mask = new_mask_from_row(r3, mask);
                    if (new_mask == -1) continue;

                    int new_state = r2 * ROWS + r3;
                    size_t nidx = (size_t)new_state * MASK_COUNT + new_mask;
                    dp_next[nidx] += cur_val;
                }
            }
        }
        /* swap dp_cur and dp_next for next iteration */
        memcpy(dp_cur, dp_next, sizeof(dp_cur));
    }

    /* --- Sum all possible final states --- */
    seq_t ans = 0;
    for (size_t i = 0; i < dim; ++i) {
        ans += dp_cur[i];
    }
    return ans;
}
