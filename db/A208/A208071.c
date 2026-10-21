/*
 * A208071  Number of 6 X n 0..1 arrays avoiding 0 0 0 and 1 1 1 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A208071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of 6×n binary arrays avoiding 000,111 horizontally
 *            * and 001,011 vertically. The implementation uses a
 *            dynamic‑programming * state machine over columns. The state
 *            consists of the last two columns; * transitions are determined
 *            by vertical and horizontal constraints. * Complexity: O(n...
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static bool init_done = false;
#define MAX_COLS 64          /* 2^6 possible columns */
#define MAX_STATES (MAX_COLS * MAX_COLS)

/* Arrays used after initialization ---------------------------- */
static int  valid_cols[MAX_COLS];           /* indices of valid columns   */
static int  vm;                            /* number of valid columns    */
static int  col_to_idx[MAX_COLS];          /* mapping column -> index    */
static int  trans_starts[MAX_STATES];       /* start offset in trans_buf   */
static int  trans_lens[MAX_STATES];         /* number of transitions      */
static int  trans_buf[MAX_COLS * MAX_COLS * MAX_COLS]; /* transition list */

/* DP buffers --------------------------------------------------- */
static seq_t dp0[MAX_STATES];
static seq_t dp1[MAX_STATES];
static seq_t *dp_cur;
static seq_t *dp_next;

/* Helper to test vertical constraint for a column ----------------- */
static bool is_valid_column(int col)
{
    /* Check each vertical triple (rows r, r+1, r+2), r=0..3 */
    for (int r = 0; r <= 3; ++r) {
        int b1 = (col >> r) & 1;
        int b2 = (col >> (r + 1)) & 1;
        int b3 = (col >> (r + 2)) & 1;
        if ((b1 == 0 && b2 == 0 && b3 == 1) ||
            (b1 == 0 && b2 == 1 && b3 == 1))
            return false;
    }
    return true;
}

/* Initialization routine --------------------------------------- */
static void init(void)
{
    if (init_done)
        return;

    /* Build list of valid columns and mapping */
    vm = 0;
    for (int i = 0; i < MAX_COLS; ++i)
        col_to_idx[i] = -1;

    for (int col = 0; col < MAX_COLS; ++col) {
        if (is_valid_column(col)) {
            valid_cols[vm] = col;
            col_to_idx[col] = vm;
            ++vm;
        }
    }

    /* Precompute transitions for each pair (c1,c2) -> allowed c3 */
    int offset = 0;
    for (int i = 0; i < vm; ++i) {
        int c1 = valid_cols[i];
        for (int j = 0; j < vm; ++j) {
            int c2 = valid_cols[j];
            int idx_state = i * vm + j;
            int count = 0;

            for (int k = 0; k < vm; ++k) {
                int c3 = valid_cols[k];
                bool ok = true;
                for (int r = 0; r < 6; ++r) {
                    int b1 = (c1 >> r) & 1;
                    int b2 = (c2 >> r) & 1;
                    int b3 = (c3 >> r) & 1;
                    if (b1 == b2 && b2 == b3) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    trans_buf[offset + count] = k;
                    ++count;
                }
            }

            trans_starts[idx_state] = offset;
            trans_lens[idx_state]   = count;
            offset += count;
        }
    }

    /* Assign DP buffers */
    dp_cur  = dp0;
    dp_next = dp1;

    init_done = true;
}

/* Main sequence function ---------------------------------------- */
seq_t a(idx_t n)
{
    init();

    if (n == 0)
        return 0;    /* not defined; offset is 1 */

    if (n == 1) {
        return (seq_t)vm;    /* first column can be any valid column */
    }

    size_t stateCount = (size_t)vm * (size_t)vm;

    /* Initialize dp for the first two columns (no horizontal check) */
    for (size_t idx = 0; idx < stateCount; ++idx)
        dp_cur[idx] = 0;
    for (int i = 0; i < vm; ++i)
        for (int j = 0; j < vm; ++j)
            dp_cur[i * vm + j] = 1;

    /* For n = 2 we skip the loop below; result will be sum of dp_cur */
    for (idx_t col = 3; col <= n; ++col) {
        /* Zero next DP array */
        for (size_t idx = 0; idx < stateCount; ++idx)
            dp_next[idx] = 0;

        for (size_t state = 0; state < stateCount; ++state) {
            seq_t val = dp_cur[state];
            if (val == 0)
                continue;

            int j = (int)(state % (size_t)vm); /* second column of the pair */
            int start = trans_starts[state];
            int len   = trans_lens[state];

            for (int t = 0; t < len; ++t) {
                int k = trans_buf[start + t];
                size_t next_state = (size_t)j * vm + k;
                dp_next[next_state] += val;
            }
        }

        /* Swap dp buffers */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* Sum all possibilities after processing n columns */
    seq_t result = 0;
    for (size_t idx = 0; idx < stateCount; ++idx)
        result += dp_cur[idx];

    return result;
}
