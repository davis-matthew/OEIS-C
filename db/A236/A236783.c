/*
 * A236783  Number of (n+1)X(2+1) 0..2 arrays with the upper median plus the
 *          lower median minus the minimum of every 2X2 subblock equal.
 * https://oeis.org/A236783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays with entries 0..2 such that for
 *            every 2×2 subblock the value upper‑median + lower‑median –
 *            minimum is the same constant throughout the array
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   blockH
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/blockH.h"

#define ROWS 27
#define HVAL 5          /* possible values of the constant: 0..4 */

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* defined only for n ≥ 1 */

    /* ---------------------------------------------
       1.  Pre‑compute all 27 row patterns and transitions
           (done only once). --------------------------------------------- */
    static int rows[ROWS][3];
    static signed char trans[ROWS][ROWS];
    static int initialized = 0;

    if (!initialized)
    {
        /* Fill rows: p = 0 .. 26, base‑3 representation. */
        for (int p = 0; p < ROWS; ++p)
        {
            int x = p;
            rows[p][2] = x % 3;  x /= 3;
            rows[p][1] = x % 3;  x /= 3;
            rows[p][0] = x % 3;
        }

        /* Compute transitions.  trans[p][q] = -1 if invalid,
           otherwise the constant (0..4) produced by the pair. */
        for (int i = 0; i < ROWS; ++i)
            for (int j = 0; j < ROWS; ++j)
                trans[i][j] = -1;

        for (int i = 0; i < ROWS; ++i)
        {
            for (int j = 0; j < ROWS; ++j)
            {
                int a0 = rows[i][0], a1 = rows[i][1], a2 = rows[i][2];
                int b0 = rows[j][0], b1 = rows[j][1], b2 = rows[j][2];
                int h0 = blockH(a0, a1, b0, b1);
                int h1 = blockH(a1, a2, b1, b2);
                if (h0 == h1)
                    trans[i][j] = (signed char)h0;   /* 0..4 */
            }
        }
        initialized = 1;
    }

    /* ---------------------------------------------
       2.  Dynamic programming over rows and the constant. --------------------------------------------- */
    static seq_t dp_prev[ROWS][HVAL];
    static seq_t dp_cur[ROWS][HVAL];

    /* Zero DP tables. */
    for (int r = 0; r < ROWS; ++r)
        for (int h = 0; h < HVAL; ++h)
        {
            dp_prev[r][h] = (seq_t)0;
            dp_cur[r][h]  = (seq_t)0;
        }

    /*  First transition: from the initial row (any pattern)
        to the second row.  Each pair produces a constant h. */
    for (int i = 0; i < ROWS; ++i)
    {
        for (int j = 0; j < ROWS; ++j)
        {
            int th = trans[i][j];
            if (th >= 0)
                dp_cur[j][th] += (seq_t)1;   /* first_row[i] == 1 */
        }
    }

    if (n == 1)   /* only one transition */
    {
        seq_t total = (seq_t)0;
        for (int r = 0; r < ROWS; ++r)
            for (int h = 0; h < HVAL; ++h)
                total += dp_cur[r][h];
        return total;
    }

    /* Prepare for subsequent steps: move dp_cur to dp_prev, clear dp_cur. */
    for (int r = 0; r < ROWS; ++r)
        for (int h = 0; h < HVAL; ++h)
        {
            dp_prev[r][h] = dp_cur[r][h];
            dp_cur[r][h]  = (seq_t)0;
        }

    /* Additional transitions: rows 1→2, 2→3, …, (n‑1)→n. */
    for (idx_t step = 2; step <= n; ++step)
    {
        for (int i = 0; i < ROWS; ++i)
        {
            for (int h = 0; h < HVAL; ++h)
            {
                seq_t val = dp_prev[i][h];
                if (val == (seq_t)0) continue;

                for (int j = 0; j < ROWS; ++j)
                {
                    int th = trans[i][j];
                    if (th == h)
                        dp_cur[j][h] += val;
                }
            }
        }
        /* Switch tables for next iteration. */
        for (int r = 0; r < ROWS; ++r)
            for (int h = 0; h < HVAL; ++h)
            {
                dp_prev[r][h] = dp_cur[r][h];
                dp_cur[r][h]  = (seq_t)0;
            }
    }

    /* Sum all final states. */
    seq_t total = (seq_t)0;
    for (int r = 0; r < ROWS; ++r)
        for (int h = 0; h < HVAL; ++h)
            total += dp_prev[r][h];

    return total;
}
