/*
 * A183470  Number of nX5 binary arrays with every element equal to either the
 *          sum mod 2 of its vertical neighbors or the sum mod 2 of its
 *          horizontal neighbors.
 * https://oeis.org/A183470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A183470 : Number of n×5 binary arrays where each entry equals
 * either the sum mod 2 of its vertical neighbours or that of its horizontal
 * neighbours.  The implementation uses dynamic programming over the rows,
 * storing transitions between successive 5‑bit rows.  No external library
 * is required beyond the standard headers allowed by the judge. */

#include <stdint.h>
#include <stdbool.h>

/* Transition tables – populated on first call */
static unsigned char NEXT_ROWS[32][32][32];   /* possible next rows */
static unsigned char NEXT_LEN[32][32];        /* number of possible next rows */
static bool START_VALID[32][32];              /* valid_row(0,a,b) */
static bool BOTTOM_VALID[32][32];             /* valid_row(a,b,0) */

/* Test whether a middle row `b` satisfies the condition when the
 * vertical neighbours are `a` (above) and `c` (below).  All rows are
 * represented by 5‑bit unsigned integers. */
static bool
valid_row(uint8_t a, uint8_t b, uint8_t c)
{
    for (int j = 0; j < 5; ++j) {
        uint8_t bj   = (b >> j) & 1;
        uint8_t hsum = ((j > 0)   ? ((b >> (j - 1)) & 1) : 0) ^
                       ((j < 4)   ? ((b >> (j + 1)) & 1) : 0);
        uint8_t vsum = ((a >> j) & 1) ^ ((c >> j) & 1);
        if (bj != hsum && bj != vsum)
            return false;
    }
    return true;
}

/* Initialise the transition tables the first time a() is called */
static void
init_transition_tables(void)
{
    static bool initialized = false;
    if (initialized)
        return;
    initialized = true;

    for (int a = 0; a < 32; ++a) {
        for (int b = 0; b < 32; ++b) {
            START_VALID[a][b] = valid_row(0, (uint8_t)a, (uint8_t)b);
            BOTTOM_VALID[a][b] = valid_row((uint8_t)a, (uint8_t)b, 0);
            int len = 0;
            for (int c = 0; c < 32; ++c) {
                if (valid_row((uint8_t)a, (uint8_t)b, (uint8_t)c)) {
                    NEXT_ROWS[a][b][len] = (unsigned char)c;
                    ++len;
                }
            }
            NEXT_LEN[a][b] = (unsigned char)len;
        }
    }
}

/* Main sequence function.  All arithmetic uses seq_t, which may be an
 * arbitrary‑precision type (or a fixed unsigned integer type).  The function
 * is order‑independent and uses memoised transition tables. */
seq_t
a(idx_t n)
{
    /* Undefined for n==0 – return 0.  For n<1 behaviour is irrelevant. */
    if (n < 1)
        return (seq_t)0;

    /* Build transition tables once. */
    init_transition_tables();

    /* Handle the trivial case n==1 directly. */
    if (n == 1) {
        seq_t cnt = 0;
        for (int row = 0; row < 32; ++row) {
            if (valid_row(0, (uint8_t)row, 0))
                cnt += 1;
        }
        return cnt;
    }

    /* DP arrays: dp_cur[a][b] = number of ways to reach a configuration
     * whose last two rows are a (previous) and b (latest). */
    seq_t dp_cur[32][32];
    seq_t dp_next[32][32];

    /* Initialise dp_cur to zero. */
    for (int a = 0; a < 32; ++a)
        for (int b = 0; b < 32; ++b)
            dp_cur[a][b] = 0;

    /* Initial pair of rows: row0 is implicit 0, row1 varies. */
    for (int r0 = 0; r0 < 32; ++r0) {
        for (int r1 = 0; r1 < 32; ++r1) {
            if (START_VALID[r0][r1])
                dp_cur[r0][r1] = 1;
        }
    }

    /* Process additional rows (row indices 2 … n‑1). */
    for (idx_t step = 2; step <= n - 1; ++step) {
        /* Clear dp_next. */
        for (int a = 0; a < 32; ++a)
            for (int b = 0; b < 32; ++b)
                dp_next[a][b] = 0;

        for (int a = 0; a < 32; ++a) {
            for (int b = 0; b < 32; ++b) {
                seq_t cur_val = dp_cur[a][b];
                if (!cur_val)
                    continue;
                int len = NEXT_LEN[a][b];
                const unsigned char *list = NEXT_ROWS[a][b];
                for (int k = 0; k < len; ++k) {
                    int c = list[k];
                    dp_next[b][c] += cur_val;
                }
            }
        }

        /* Swap dp_cur and dp_next by copying. */
        for (int a = 0; a < 32; ++a)
            for (int b = 0; b < 32; ++b)
                dp_cur[a][b] = dp_next[a][b];
    }

    /* Sum over final states that satisfy the bottom condition. */
    seq_t result = 0;
    for (int a = 0; a < 32; ++a) {
        for (int b = 0; b < 32; ++b) {
            if (dp_cur[a][b] && BOTTOM_VALID[a][b])
                result += dp_cur[a][b];
        }
    }

    return result;
}
