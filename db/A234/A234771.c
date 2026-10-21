/*
 * A234771  Number of (n+1)X(2+1) 0..4 arrays with 2X2 edge jumps all no more
 *          than +1 in one of the clockwise or counterclockwise directions but
 *          not both.
 * https://oeis.org/A234771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by dynamic programming over (n+1)-row 3‑column arrays
 * with entries 0..4 satisfying the edge‑jump condition on every
 * 2×2 subarray.  The DP counts all valid sequences of rows of
 * length n+1. */

#include <stdbool.h>

static bool compat_init = false;
static bool compat[125][125];   /* compat[i][j] = true iff rows i and j can be adjacent */

/* Build a base‑5 representation of each row (three digits 0..4). */
static void init_compat(void)
{
    uint8_t digits[125][3];
    for (int i = 0; i < 125; ++i)
    {
        int v = i;
        for (int k = 2; k >= 0; --k)
        {
            digits[i][k] = v % 5;
            v /= 5;
        }
    }

    for (int i = 0; i < 125; ++i)
    {
        for (int j = 0; j < 125; ++j)
        {
            bool ok = true;
            for (int c = 0; c < 2; ++c)
            {
                int a0 = digits[i][c];
                int a1 = digits[i][c + 1];
                int b0 = digits[j][c];
                int b1 = digits[j][c + 1];

                /* Clockwise edges */
                int cw1 = a1 - a0;
                int cw2 = b1 - a1;
                int cw3 = b0 - b1;
                int cw4 = a0 - b0;
                bool cw = (cw1 <= 1 && cw2 <= 1 && cw3 <= 1 && cw4 <= 1);

                /* Counter‑clockwise edges */
                int ccw1 = b0 - a0;
                int ccw2 = b1 - b0;
                int ccw3 = a1 - b1;
                int ccw4 = a0 - a1;
                bool ccw = (ccw1 <= 1 && ccw2 <= 1 && ccw3 <= 1 && ccw4 <= 1);

                if (!(cw ^ ccw)) { ok = false; break; }
            }
            compat[i][j] = ok;
        }
    }
    compat_init = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* not required for the offset */
    if (!compat_init) init_compat();

    const int S = 125;
    seq_t cur[S];
    seq_t next[S];

    for (int i = 0; i < S; ++i) cur[i] = 1;   /* first row can be any */

    for (idx_t step = 0; step < n; ++step)    /* n transitions => n+1 rows */
    {
        for (int j = 0; j < S; ++j) next[j] = 0;
        for (int i = 0; i < S; ++i)
        {
            if (cur[i] == 0) continue;
            for (int j = 0; j < S; ++j)
                if (compat[i][j]) next[j] += cur[i];
        }
        for (int i = 0; i < S; ++i) cur[i] = next[i];
    }

    seq_t total = 0;
    for (int i = 0; i < S; ++i) total += cur[i];
    return total;
}
