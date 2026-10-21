/*
 * A231058  Number of n X 3 0..3 arrays x(i,j) with each element horizontally
 *          or vertically next to at least one element with value (x(i,j)+1)
 *          mod 4, no adjacent elements equal, and upper left element zero.
 * https://oeis.org/A231058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 arrays x(i,j) with * each element
 *            horizontally or vertically next to at least one element * with
 *            value (x(i,j)+1) mod 4, no adjacent elements equal, and upper *
 *            left element zero. * * The algorithm uses dynamic programming
 *            over rows. Each state * encodes the previous row's co...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Static arrays for dynamic programming.  Two buffers are used
     * and swapped each iteration to avoid copying big integer objects. */
    static seq_t dp[512];
    static seq_t ndp[512];
    seq_t *curdp = dp, *nxtdp = ndp;
    int i;

    /* Initialise dp to zero. */
    for (i = 0; i < 512; ++i) curdp[i] = (seq_t)0;

    /* ----- Row 0 (first row) ----- */
    for (unsigned row = 0; row < 64; ++row)
    {
        int a = row >> 4;       /* column 0 */
        int b = (row >> 2) & 3; /* column 1 */
        int c = row & 3;        /* column 2 */

        /* Upper‑left element fixed to 0. */
        if (a != 0) continue;

        /* No horizontal equality. */
        if (a == b || b == c) continue;

        /* Compute unsatisfied cells using only left/right neighbors. */
        int need = 0;
        /* Cell 0: only right neighbor (b). */
        if (b != ((a + 1) & 3)) need |= 1;
        /* Cell 1: left (a) or right (c). */
        if (a != ((b + 1) & 3) && c != ((b + 1) & 3)) need |= 2;
        /* Cell 2: only left neighbor (b). */
        if (b != ((c + 1) & 3)) need |= 4;

        int colors = a * 16 + b * 4 + c;
        int state  = colors * 8 + need;
        curdp[state] += (seq_t)1;
    }

    /* If only one row, sum states with all cells satisfied. */
    if (n == 1)
    {
        seq_t total = (seq_t)0;
        for (i = 0; i < 512; ++i)
            if ((i & 7) == 0) total += curdp[i];
        return total;
    }

    /* ----- Subsequent rows (row 1 … n-1) ----- */
    for (idx_t row = 1; row < n; ++row)
    {
        /* Zero next buffer. */
        for (i = 0; i < 512; ++i) nxtdp[i] = (seq_t)0;

        /* Process all states from previous row. */
        for (i = 0; i < 512; ++i)
        {
            seq_t val = curdp[i];
            if (val == (seq_t)0) continue;

            int need_prev = i & 7;
            int colors    = i >> 3;
            int pa = colors >> 4;
            int pb = (colors >> 2) & 3;
            int pc = colors & 3;

            for (unsigned cur = 0; cur < 64; ++cur)
            {
                int ca = cur >> 4;
                int cb = (cur >> 2) & 3;
                int cc = cur & 3;

                /* No horizontal equality. */
                if (ca == cb || cb == cc) continue;

                /* No vertical equality. */
                if (ca == pa || cb == pb || cc == pc) continue;

                /* Previous row cells that still need a next neighbor. */
                if ((need_prev & 1) && ca != ((pa + 1) & 3)) continue;
                if ((need_prev & 2) && cb != ((pb + 1) & 3)) continue;
                if ((need_prev & 4) && cc != ((pc + 1) & 3)) continue;

                /* Compute unsatisfied cells in the current row. */
                int need_cur = 0;

                /* Cell 0 */
                int sat0 = 0;
                if (cb == ((ca + 1) & 3)) sat0 = 1;    /* right neighbor */
                if (pa == ((ca + 1) & 3)) sat0 = 1;    /* up neighbor */
                if (!sat0) need_cur |= 1;

                /* Cell 1 */
                int sat1 = 0;
                if (ca == ((cb + 1) & 3)) sat1 = 1;    /* left   */
                if (cc == ((cb + 1) & 3)) sat1 = 1;    /* right  */
                if (pb == ((cb + 1) & 3)) sat1 = 1;    /* up     */
                if (!sat1) need_cur |= 2;

                /* Cell 2 */
                int sat2 = 0;
                if (cb == ((cc + 1) & 3)) sat2 = 1;    /* left   */
                if (pc == ((cc + 1) & 3)) sat2 = 1;    /* up     */
                if (!sat2) need_cur |= 4;

                int colors_new = ca * 16 + cb * 4 + cc;
                int state_new  = colors_new * 8 + need_cur;
                nxtdp[state_new] += val;
            }
        }

        /* Swap buffers for next iteration. */
        seq_t *tmp = curdp;
        curdp     = nxtdp;
        nxtdp     = tmp;
    }

    /* Sum all final states where every cell is satisfied. */
    seq_t total = (seq_t)0;
    for (i = 0; i < 512; ++i)
        if ((i & 7) == 0) total += curdp[i];
    return total;
}
