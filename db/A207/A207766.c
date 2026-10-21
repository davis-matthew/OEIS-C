/*
 * A207766  Number of 6 X n 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 1 1 and 1 0 1 vertically.
 * https://oeis.org/A207766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 14, u64 n <= 32, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A207766
 * Count of 6 × n binary arrays avoiding horizontal patterns 000 and 101
 *   and vertical patterns 011 and 101.
 * Computed by dynamic programming on pairs of consecutive columns.
 * For n ≥ 1 the result is returned in result type seq_t.
 */

#include <stddef.h>
#include <stdint.h>

/* ----- Global tables (built once) --------------------------------------- */

#define MAX_COLS  64                  /* all 6‑bit columns */
#define MAX_STATES (MAX_COLS * MAX_COLS) /* at most 4096 states */

static unsigned char valid_cols[MAX_COLS];       /* collected vertical‑valid columns */
static unsigned int   col_count;                /* number of such columns */

static int state_index[MAX_COLS][MAX_COLS];   /* (col1,col2) -> state id (-1) */
static unsigned int   state_count;            /* total number of states */

static unsigned int next_state[MAX_STATES][MAX_COLS];
static unsigned char next_deg[MAX_STATES];

static seq_t dp_a[MAX_STATES];
static seq_t dp_b[MAX_STATES];
static seq_t *dp_curr;
static seq_t *dp_next;

static int initialized = 0;

/* ----- Helpers ---------------------------------------------------------- */

static inline int vertical_ok(int col)
{
    /* disallow vertical patterns 011 (3) and 101 (5) inside a column   */
    for (int r = 0; r <= 3; ++r) {
        int pat = ((col >> r) & 1) << 2 |
                  ((col >> (r + 1)) & 1) << 1 |
                  ((col >> (r + 2)) & 1);
        if (pat == 3 || pat == 5)
            return 0;
    }
    return 1;
}

static inline int horizontal_ok(int a, int b, int c)
{
    /* disallow horizontal patterns 000 and 101 in any row   */
    for (int r = 0; r < 6; ++r) {
        int pat = ((a >> r) & 1) << 2 |
                  ((b >> r) & 1) << 1 |
                  ((c >> r) & 1);
        if (pat == 0 || pat == 5)
            return 0;
    }
    return 1;
}

/* ----- Init tables ------------------------------------------------------- */

static void init(void)
{
    if (initialized) return;
    initialized = 1;

    /* collect all columns that satisfy the vertical condition      */
    col_count = 0;
    for (int col = 0; col < MAX_COLS; ++col)
        if (vertical_ok(col))
            valid_cols[col_count++] = (unsigned char)col;

    /* initialise state_index with -1 */
    for (int i = 0; i < MAX_COLS; ++i)
        for (int j = 0; j < MAX_COLS; ++j)
            state_index[i][j] = -1;

    /* assign state id to each pair (col_i, col_j) of valid columns   */
    state_count = 0;
    for (int i = 0; i < col_count; ++i)
        for (int j = 0; j < col_count; ++j)
            state_index[valid_cols[i]][valid_cols[j]] = (int)state_count++;

    /* build transition lists: for each state (c1,c2) list possible c3   */
    for (int s = 0; s < state_count; ++s)
        next_deg[s] = 0;

    for (int i = 0; i < col_count; ++i)
        for (int j = 0; j < col_count; ++j) {
            int col1 = valid_cols[i];
            int col2 = valid_cols[j];
            int s = state_index[col1][col2];
            for (int k = 0; k < col_count; ++k) {
                int col3 = valid_cols[k];
                if (horizontal_ok(col1, col2, col3)) {
                    int t = state_index[col2][col3];
                    if (t >= 0) {
                        next_state[s][next_deg[s]++] = (unsigned int)t;
                    }
                }
            }
        }

    dp_curr = dp_a;
    dp_next = dp_b;
}

/* ----- Public sequence function ----------------------------------------- */

seq_t a(idx_t n)
{
    init();

    if (n == 0) return (seq_t)0;           /* undefined, handled safely   */
    if (n == 1) return (seq_t)col_count;   /* single column                     */
    if (n == 2) return (seq_t)col_count * (seq_t)col_count;

    /* initialise all states for the first two columns (each choice ≡1)   */
    for (unsigned int s = 0; s < state_count; ++s)
        dp_curr[s] = (seq_t)1;

    /* add columns 3 … n, one by one                                  */
    for (idx_t col = 3; col <= n; ++col) {
        /* clear next buffer                                       */
        for (unsigned int s = 0; s < state_count; ++s)
            dp_next[s] = (seq_t)0;

        /* transition from every state                              */
        for (unsigned int s = 0; s < state_count; ++s) {
            seq_t val = dp_curr[s];
            if (val == 0) continue;
            unsigned char d = next_deg[s];
            for (unsigned char k = 0; k < d; ++k) {
                unsigned int t = next_state[s][k];
                dp_next[t] += val;
            }
        }
        /* swap current and next buffers                           */
        seq_t *tmp = dp_curr;
        dp_curr   = dp_next;
        dp_next   = tmp;
    }

    /* sum all states for the final column pair                    */
    seq_t total = (seq_t)0;
    for (unsigned int s = 0; s < state_count; ++s)
        total += dp_curr[s];

    return total;
}
