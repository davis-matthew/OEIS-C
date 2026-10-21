/*
 * A207010  Number of nX3 0..2 arrays avoiding the patterns z z+1 z or z z-1 z
 *          in any row or column.
 * https://oeis.org/A207010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 arrays over {0,1,2} avoiding the patterns z
 *            z+1 z or z z-1 z in any row or column. Computation via dynamic
 *            programming over allowed row states
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* All constants depend on the problem description. */
#define ALLOWED_ROWS 23
#define STATE_COUNT  (ALLOWED_ROWS * ALLOWED_ROWS)

/* Representation of each allowed row as three digits 0..2. */
static uint8_t row_digits[ALLOWED_ROWS][3];

/* For each ordered pair (p,q), store the list of rows r that can follow
   without creating a forbidden vertical pattern. */
static uint8_t next_rows[STATE_COUNT][ALLOWED_ROWS];
static uint8_t next_count[STATE_COUNT];

static bool initialized = false;

/* Lazily build the transition tables once. */
static void init_tables(void)
{
    if (initialized) return;

    /* 1. Enumerate all allowed rows (exclude horizontal forbidden patterns). */
    int r = 0;
    for (int d0 = 0; d0 < 3; ++d0)
        for (int d1 = 0; d1 < 3; ++d1)
            for (int d2 = 0; d2 < 3; ++d2)
            {
                if ((d0 == 0 && d1 == 1 && d2 == 0) ||
                    (d0 == 1 && d1 == 0 && d2 == 1) ||
                    (d0 == 1 && d1 == 2 && d2 == 1) ||
                    (d0 == 2 && d1 == 1 && d2 == 2))
                    continue;
                row_digits[r][0] = (uint8_t)d0;
                row_digits[r][1] = (uint8_t)d1;
                row_digits[r][2] = (uint8_t)d2;
                ++r;
            }

    /* 2. Precompute transitions for each state (p,q). */
    for (int p = 0; p < ALLOWED_ROWS; ++p)
        for (int q = 0; q < ALLOWED_ROWS; ++q)
        {
            int state = p * ALLOWED_ROWS + q;
            uint8_t count = 0;
            for (int rr = 0; rr < ALLOWED_ROWS; ++rr)
            {
                bool ok = true;
                for (int col = 0; col < 3 && ok; ++col)
                {
                    int a = row_digits[p][col];
                    int b = row_digits[q][col];
                    int c = row_digits[rr][col];
                    if (a == c && (b == a + 1 || b == a - 1))
                        ok = false;
                }
                if (ok)
                {
                    next_rows[state][count] = (uint8_t)rr;
                    ++count;
                }
            }
            next_count[state] = count;
        }

    initialized = true;
}

/* Main function computing a(n). */
seq_t a(idx_t n)
{
    /* Base cases. */
    if (n == 1) return (seq_t)23;
    if (n == 2) return (seq_t)529;

    init_tables();

    /* DP arrays for states. */
    static seq_t prev[STATE_COUNT];
    static seq_t curr[STATE_COUNT];

    /* Initialize for n = 2: all pairs are possible. */
    for (int i = 0; i < STATE_COUNT; ++i)
        prev[i] = (seq_t)1;

    idx_t steps = n - 2;   /* number of rows to add after the first two. */

    for (idx_t s = 0; s < steps; ++s)
    {
        /* Zero current counts. */
        for (int i = 0; i < STATE_COUNT; ++i)
            curr[i] = (seq_t)0;

        /* Transition from each state. */
        for (int state = 0; state < STATE_COUNT; ++state)
        {
            seq_t cnt = prev[state];
            if (cnt == 0) continue;

            uint8_t num_next = next_count[state];
            for (uint8_t k = 0; k < num_next; ++k)
            {
                int rr = next_rows[state][k];
                int next_state = (state % ALLOWED_ROWS) * ALLOWED_ROWS + rr;
                curr[next_state] += cnt;
            }
        }

        /* Swap prev and curr for next iteration. */
        for (int i = 0; i < STATE_COUNT; ++i)
            prev[i] = curr[i];
    }

    /* Sum all final states. */
    seq_t result = (seq_t)0;
    for (int i = 0; i < STATE_COUNT; ++i)
        result += prev[i];

    return result;
}
