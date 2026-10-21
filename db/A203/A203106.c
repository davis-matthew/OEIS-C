/*
 * A203106  Number of nX6 binary arrays with every 1 immediately preceded by 0
 *          0 to the left or above.
 * https://oeis.org/A203106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 binary arrays in which every 1 is
 *            immediately preceded by two 0's to the left or by two 0's above
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: To handle the special situation that the first two rows have no
   two rows above, the DP treats the first two rows differently: only
   the horizontal rule is enforced for them.  From the third row onwards
   both horizontal and vertical rules are applied. */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Constants */
#define COLS 6
#define ROWS 64
#define STATE_BITS 12
#define STATE_SIZE (1U << STATE_BITS)

/* Precomputed tables */
static unsigned char h_patterns[ROWS];
static unsigned char h_count;

static unsigned char next_count[STATE_SIZE];
static unsigned char next_rows[STATE_SIZE][ROWS];

static bool tables_built = false;

/* Compute whether a 6‑bit row satisfies the horizontal rule. */
static bool horizontal_ok(unsigned char row)
{
    for (int j = 0; j < COLS; ++j)
    {
        if ((row >> j) & 1U)
        {
            if (j < 2)        /* need two positions to the left */
                return false;
            if (((row >> (j - 1)) & 1U) || ((row >> (j - 2)) & 1U))
                return false;
        }
    }
    return true;
}

/* Check whether a transition from (prev2,prev) to new_row satisfies
   the vertical rule (two zeros above) or the horizontal rule. */
static bool valid_transition(unsigned char prev2, unsigned char prev,
                             unsigned char new_row)
{
    for (int j = 0; j < COLS; ++j)
    {
        if ((new_row >> j) & 1U)
        {
            bool horiz = false;
            if (j >= 2)
            {
                if (((new_row >> (j - 1)) & 1U) == 0U &&
                    ((new_row >> (j - 2)) & 1U) == 0U)
                    horiz = true;
            }
            bool vert = (((prev >> j) & 1U) == 0U &&
                         ((prev2 >> j) & 1U) == 0U);
            if (!horiz && !vert)
                return false;
        }
    }
    return true;
}

/* Build the tables of horizontal patterns and full transitions. */
static void build_tables(void)
{
    /* 1. Horizontal patterns. */
    h_count = 0;
    for (int r = 0; r < ROWS; ++r)
        if (horizontal_ok((unsigned char)r))
            h_patterns[h_count++] = (unsigned char)r;

    /* 2. Full transitions. */
    for (int s = 0; s < STATE_SIZE; ++s)
    {
        unsigned char prev2 = (unsigned char)(s >> COLS);
        unsigned char prev  = (unsigned char)(s & 0x3F);
        unsigned char *dest = next_rows[s];
        unsigned char nk = 0;
        for (int r = 0; r < ROWS; ++r)
        {
            if (valid_transition(prev2, prev, (unsigned char)r))
                dest[nk++] = (unsigned char)r;
        }
        next_count[s] = nk;
    }
    tables_built = true;
}

/* Main DP routine. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    if (!tables_built)
        build_tables();

    static seq_t dp_prev[STATE_SIZE];
    static seq_t dp_next[STATE_SIZE];
    size_t act_prev[STATE_SIZE];
    size_t act_next[STATE_SIZE];
    size_t act_prev_cnt = 0, act_next_cnt = 0;

    /* Initial state: two imaginary zero rows. */
    memset(dp_prev, 0, sizeof(dp_prev));
    dp_prev[0] = 1;
    act_prev[act_prev_cnt++] = 0;

    for (idx_t row = 1; row <= n; ++row)
    {
        memset(dp_next, 0, sizeof(dp_next));
        act_next_cnt = 0;

        if (row <= 2)   /* only horizontal rule applies */
        {
            for (size_t ai = 0; ai < act_prev_cnt; ++ai)
            {
                size_t s = act_prev[ai];
                seq_t ways = dp_prev[s];
                if (ways == 0)
                    continue;
                unsigned char prev_row = (unsigned char)(s & 0x3F);
                for (unsigned char k = 0; k < h_count; ++k)
                {
                    unsigned char r = h_patterns[k];
                    unsigned short new_state = (prev_row << COLS) | r;
                    if (dp_next[new_state] == 0)
                        act_next[act_next_cnt++] = new_state;
                    dp_next[new_state] += ways;
                }
            }
        }
        else            /* full rule applies */
        {
            for (size_t ai = 0; ai < act_prev_cnt; ++ai)
            {
                size_t s = act_prev[ai];
                seq_t ways = dp_prev[s];
                if (ways == 0)
                    continue;
                unsigned char prev_row = (unsigned char)(s & 0x3F);
                unsigned char kmax = next_count[s];
                unsigned char *allowed = next_rows[s];
                for (unsigned char k = 0; k < kmax; ++k)
                {
                    unsigned char r = allowed[k];
                    unsigned short new_state = (prev_row << COLS) | r;
                    if (dp_next[new_state] == 0)
                        act_next[act_next_cnt++] = new_state;
                    dp_next[new_state] += ways;
                }
            }
        }

        /* Prepare for next iteration: copy dp_next to dp_prev. */
        memcpy(dp_prev, dp_next, sizeof(dp_prev));
        act_prev_cnt = act_next_cnt;
        memcpy(act_prev, act_next, act_next_cnt * sizeof(size_t));
    }

    /* Sum all final counts. */
    seq_t ans = 0;
    for (size_t i = 0; i < act_prev_cnt; ++i)
        ans += dp_prev[act_prev[i]];
    return ans;
}
