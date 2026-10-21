/*
 * A231243  Number of nX5 0..3 arrays x(i,j) with each element horizontally,
 *          diagonally or antidiagonally next to at least one element with
 *          value (x(i,j)+1) mod 4, no adjacent elements equal, and upper left
 *          element zero.
 * https://oeis.org/A231243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A231243 – Number of n×5 0..3 arrays with the described constraints.
   The algorithm uses a state‑DP over rows.
   Each state is a 5‑tuple of values 0..3 with no horizontal equal adjacency.
   Transitions enforce diagonal inequalities and the “value+1” neighbour rule.
   The DP stores for each row the set of cells already satisfied by left/right
   and up neighbours.  A transition is allowed only if the previous row becomes
   fully satisfied by the added down neighbours.  The final row must also be
   completely satisfied (only left/right and up neighbours exist). */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

enum { NUM_COLS = 5 };
enum { STATE_COUNT = 324 };    /* 4*3^4 */

static unsigned char state_vals[STATE_COUNT][NUM_COLS];
static uint8_t        local_mask[STATE_COUNT];   /* bits set when a cell has a left/right neighbour with (v+1)%4 */
static bool           row0_flag[STATE_COUNT];   /* true when state[0]==0 */
static uint8_t        trans_len[STATE_COUNT];
static uint16_t       trans_list[STATE_COUNT][STATE_COUNT];
static uint8_t        down_mask[STATE_COUNT][STATE_COUNT];
static uint8_t        up_mask[STATE_COUNT][STATE_COUNT];
static bool           tables_initialized = false;

/* ------------------------------------------------------------------ */
/*  Helper to compute down_/up_ masks for a pair of states                 */
/* ------------------------------------------------------------------ */
static inline void compute_masks(const unsigned char *p, const unsigned char *c,
                                 uint8_t *down, uint8_t *up)
{
    uint8_t d = 0, u = 0;
    for (unsigned j = 0; j < NUM_COLS; ++j)
    {
        unsigned pv = p[j], cv = c[j];
        /* Down neighbour: check c[j-1] or c[j+1] equal to (pv+1)%4 */
        if (j > 0   && c[j - 1] == (pv + 1) % 4) d |= (1U << j);
        if (j + 1 < NUM_COLS && c[j + 1] == (pv + 1) % 4) d |= (1U << j);
        /* Up neighbour: check p[j-1] or p[j+1] equal to (cv+1)%4 */
        if (j > 0   && p[j - 1] == (cv + 1) % 4) u |= (1U << j);
        if (j + 1 < NUM_COLS && p[j + 1] == (cv + 1) % 4) u |= (1U << j);
    }
    *down = d;
    *up   = u;
}

/* ------------------------------------------------------------------ */
/*  Precomputation of all states, masks and transitions                 */
/* ------------------------------------------------------------------ */
static void init_tables(void)
{
    /* Enumerate all 5‑tuples with no horizontal equal adjacency     */
    unsigned idx = 0;
    for (unsigned a = 0; a < 4; ++a)
        for (unsigned b = 0; b < 4; ++b) if (b != a)
            for (unsigned c = 0; c < 4; ++c) if (c != b)
                for (unsigned d = 0; d < 4; ++d) if (d != c)
                    for (unsigned e = 0; e < 4; ++e) if (e != d)
                    {
                        state_vals[idx][0] = (unsigned char)a;
                        state_vals[idx][1] = (unsigned char)b;
                        state_vals[idx][2] = (unsigned char)c;
                        state_vals[idx][3] = (unsigned char)d;
                        state_vals[idx][4] = (unsigned char)e;
                        ++idx;
                    }
    /* idx == STATE_COUNT (324) */

    /* Compute local masks and row0 flag */
    for (unsigned s = 0; s < STATE_COUNT; ++s)
    {
        uint8_t m = 0;
        for (unsigned j = 0; j < NUM_COLS; ++j)
        {
            unsigned v = state_vals[s][j];
            if (j > 0     && state_vals[s][j - 1] == (v + 1) % 4) m |= (1U << j);
            if (j + 1 < NUM_COLS && state_vals[s][j + 1] == (v + 1) % 4) m |= (1U << j);
        }
        local_mask[s] = m;
        row0_flag[s]  = (state_vals[s][0] == 0);
    }

    /* Compute transitions and down/up masks */
    for (unsigned s = 0; s < STATE_COUNT; ++s)
    {
        trans_len[s] = 0;
        for (unsigned t = 0; t < STATE_COUNT; ++t)
        {
            bool ok = true;
            for (unsigned j = 0; j < NUM_COLS && ok; ++j)
            {
                unsigned sv = state_vals[s][j];
                if (j + 1 < NUM_COLS && sv == state_vals[t][j + 1]) ok = false;
                if (j > 0   && sv == state_vals[t][j - 1])              ok = false;
            }
            if (!ok) continue;

            trans_list[s][trans_len[s]++] = (uint16_t)t;

            uint8_t down, up;
            compute_masks(state_vals[s], state_vals[t], &down, &up);
            down_mask[s][t] = down;
            up_mask[t][s]   = up;   /* note the swapped indices */
        }
    }
    tables_initialized = true;
}

/* ------------------------------------------------------------------ */
/*  Main routine                                                      */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!tables_initialized) init_tables();

    if (n == 0) return (seq_t)0;   /* not defined, return 0 */

    const unsigned ALL = (1U << NUM_COLS) - 1U;   /* 0x1F */

    /* Handle n==1 directly */
    if (n == 1)
    {
        seq_t total = 0;
        for (unsigned s = 0; s < STATE_COUNT; ++s)
            if (row0_flag[s] && local_mask[s] == ALL)
                ++total;
        return total;
    }

    /* DP arrays (static to avoid stack overflow) */
    static seq_t dp[STATE_COUNT][32];
    static seq_t ndp[STATE_COUNT][32];

    /* Initialise dp for the first row */
    for (unsigned s = 0; s < STATE_COUNT; ++s)
        for (unsigned m = 0; m < 32; ++m)
            dp[s][m] = 0;
    for (unsigned s = 0; s < STATE_COUNT; ++s)
        if (row0_flag[s])
            dp[s][local_mask[s]] = 1;

    /* Process rows 2..n */
    for (idx_t row = 1; row < n; ++row)
    {
        for (unsigned s = 0; s < STATE_COUNT; ++s)
            for (unsigned m = 0; m < 32; ++m)
                ndp[s][m] = 0;

        for (unsigned s = 0; s < STATE_COUNT; ++s)
        {
            for (unsigned m = 0; m < 32; ++m)
            {
                seq_t cnt = dp[s][m];
                if (cnt == 0) continue;

                for (unsigned ii = 0; ii < trans_len[s]; ++ii)
                {
                    unsigned t = trans_list[s][ii];
                    uint8_t down = down_mask[s][t];
                    uint8_t new_prev_mask = m | down;
                    if (new_prev_mask != ALL) continue;   /* previous row not fully satisfied */

                    uint8_t curr_mask = local_mask[t] | up_mask[t][s];
                    ndp[t][curr_mask] += cnt;
                }
            }
        }

        /* Copy ndp -> dp for next iteration */
        for (unsigned s = 0; s < STATE_COUNT; ++s)
            for (unsigned m = 0; m < 32; ++m)
                dp[s][m] = ndp[s][m];
    }

    /* Sum over fully satisfied final rows */
    seq_t result = 0;
    for (unsigned s = 0; s < STATE_COUNT; ++s)
        result += dp[s][ALL];
    return result;
}
