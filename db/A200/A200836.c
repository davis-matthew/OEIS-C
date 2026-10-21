/*
 * A200836  Number of 0..6 arrays x(0..n+1) of n+2 elements without any two
 *          consecutive increases or two consecutive decreases.
 * https://oeis.org/A200836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 0..6 arrays x(0..n+1) of n+2 elements without
 *            two consecutive increases or two consecutive decreases.
 *            Recurrence: a(n) = sum_{i,j} T_{n+2, i, j} where T_{m, i, j} is
 *            the number of admissible arrays of length m ending with pair (i,
 *            j). T_{2, i, j} = 1 for all i,j; T_{m+1, j, k} =...
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Assumes seq_t and idx_t are defined by the build environment. */

static bool allowed_table[7][7][7];
static bool allowed_init = false;

static void init_allowed_table(void)
{
    for (idx_t a = 0; a < 7; ++a)
        for (idx_t b = 0; b < 7; ++b)
            for (idx_t c = 0; c < 7; ++c)
            {
                bool inc = (a < b) && (b < c);
                bool dec = (a > b) && (b > c);
                allowed_table[a][b][c] = !(inc || dec);
            }
    allowed_init = true;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* Undefined for below offset */

    if (!allowed_init) init_allowed_table();

    /* dp_cur[i][j] stores the number of admissible arrays ending with
       the last two elements (i, j).  For length 2 every pair is valid. */
    seq_t dp_cur[7][7];
    seq_t dp_next[7][7];

    for (idx_t i = 0; i < 7; ++i)
        for (idx_t j = 0; j < 7; ++j)
            dp_cur[i][j] = 1;          /* length = 2 */

    /* Perform n steps to reach length n+2. */
    for (idx_t step = 0; step < n; ++step)
    {
        /* Zero dp_next */
        for (idx_t i = 0; i < 7; ++i)
            for (idx_t j = 0; j < 7; ++j)
                dp_next[i][j] = 0;

        for (idx_t a0 = 0; a0 < 7; ++a0)
            for (idx_t b0 = 0; b0 < 7; ++b0)
            {
                seq_t val = dp_cur[a0][b0];
                if (val == 0) continue;
                for (idx_t c0 = 0; c0 < 7; ++c0)
                    if (allowed_table[a0][b0][c0])
                        dp_next[b0][c0] += val;
            }

        /* Update dp_cur for next iteration */
        for (idx_t i = 0; i < 7; ++i)
            for (idx_t j = 0; j < 7; ++j)
                dp_cur[i][j] = dp_next[i][j];
    }

    /* Sum all entries of dp_cur to obtain a(n). */
    seq_t sum = 0;
    for (idx_t i = 0; i < 7; ++i)
        for (idx_t j = 0; j < 7; ++j)
            sum += dp_cur[i][j];

    return sum;
}
