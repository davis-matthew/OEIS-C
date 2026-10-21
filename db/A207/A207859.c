/*
 * A207859  Number of 4Xn 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A207859:
   Number of 4×n binary arrays avoiding
   horizontal patterns 0 0 0 and 0 1 0,
   and vertical patterns 0 1 0 and 1 0 1.
   The algorithm uses a state DP on pairs of columns.
   Each state is (prev_col_index, curr_col_index).  Transitions
   to (curr_col_index, next_col_index) are allowed when the
   three consecutive columns satisfy the horizontal constraints.
   Complexity: O(n m³) with m = 10 (valid columns), i.e. O(n).  */

/* Required types */
#include <stdint.h>
#include <stdbool.h>

/* 4-bit column is stored as a uint8_t mask 0..15 */
#define MAX_COLS 16          /* 2⁴ */
#define MAX_STATES (MAX_COLS * MAX_COLS)   /* 256 */

/* Valid column masks (satisfying vertical constraints) */
static uint8_t valid_cols[MAX_COLS];
static unsigned m;                /* number of valid columns */

/* Transition tables */
static unsigned next_cnt[MAX_STATES];
static unsigned next_state[MAX_STATES][MAX_COLS];

/* ------------------------------------------------------------ */
/* Check a single column for vertical patterns 010 and 101      */
static bool col_ok(uint8_t s)
{
    /* rows 0‑2 and 1‑3 */
    for (int r = 0; r < 2; ++r)
    {
        int a = (s >> r) & 1;
        int b = (s >> (r + 1)) & 1;
        int c = (s >> (r + 2)) & 1;
        if ((a == 0 && b == 1 && c == 0) ||
            (a == 1 && b == 0 && c == 1))
            return false;
    }
    return true;
}

/* Check three columns for horizontal patterns 000 and 010   */
static bool triple_ok(uint8_t a, uint8_t b, uint8_t c)
{
    for (int r = 0; r < 4; ++r)
    {
        int a_b = (a >> r) & 1;
        int b_b = (b >> r) & 1;
        int c_b = (c >> r) & 1;
        if ((a_b == 0 && b_b == 0 && c_b == 0) ||
            (a_b == 0 && b_b == 1 && c_b == 0))
            return false;
    }
    return true;
}

/* Build the transition graph once                                 */
static void init_state(void)
{
    /* List valid columns                               */
    m = 0;
    for (int s = 0; s < MAX_COLS; ++s)
        if (col_ok((uint8_t)s))
            valid_cols[m++] = (uint8_t)s;

    unsigned N = m * m;   /* number of states */

    for (unsigned p = 0; p < N; ++p)
    {
        unsigned idx_a = p / m;        /* index of previous column */
        unsigned idx_b = p % m;        /* index of current column  */
        uint8_t a = valid_cols[idx_a];
        uint8_t b = valid_cols[idx_b];
        next_cnt[p] = 0;

        for (unsigned idx_c = 0; idx_c < m; ++idx_c)
        {
            uint8_t c = valid_cols[idx_c];
            if (triple_ok(a, b, c))
            {
                unsigned t = idx_b * m + idx_c;   /* state (b,c) */
                next_state[p][next_cnt[p]++] = t;
            }
        }
    }
}

/* ------------------------------------------------------------ */
/* Main sequence function                                         */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized)
    {
        init_state();
        initialized = true;
    }

    /* n == 0 not defined – return 0 as safe fallback              */
    if (n == 0) return (seq_t)0;

    /* n == 1 : single column -> each valid column counts once    */
    if (n == 1) return (seq_t)m;

    unsigned N = m * m;   /* number of states */

    /* n == 2 : any pair of valid columns                          */
    if (n == 2) return (seq_t)m * (seq_t)m;

    /* DP arrays for path counts                                  */
    static seq_t dp[MAX_STATES];
    static seq_t ndp[MAX_STATES];

    /* start with one way to have each pair (a,b) as columns 1-2    */
    for (unsigned i = 0; i < N; ++i)
        dp[i] = (seq_t)1;

    /* iterate over remaining columns 3 … n                          */
    for (unsigned col = 3; col <= n; ++col)
    {
        /* clear ndp for indices < N                                 */
        for (unsigned i = 0; i < N; ++i)
            ndp[i] = (seq_t)0;

        for (unsigned p = 0; p < N; ++p)
        {
            seq_t val = dp[p];
            if (val == 0) continue;

            unsigned cnt = next_cnt[p];
            for (unsigned k = 0; k < cnt; ++k)
            {
                unsigned t = next_state[p][k];
                ndp[t] += val;
            }
        }
        /* swap dp and ndp                                            */
        for (unsigned i = 0; i < N; ++i)
            dp[i] = ndp[i];
    }

    /* sum all paths of length n (states after processing n columns) */
    seq_t ans = (seq_t)0;
    for (unsigned i = 0; i < N; ++i)
        ans += dp[i];

    return ans;
}
