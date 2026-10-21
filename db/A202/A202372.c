/*
 * A202372  Number of (n+2)X4 binary arrays avoiding patterns 001 and 111 in
 *          rows and columns.
 * https://oeis.org/A202372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of (n+2)×4 binary arrays avoiding the
 *            row/column patterns 001 and 111. The sequence is computed by a
 *            transfer‑matrix DP that enumerates the 9 admissible row patterns
 *            and applies the vertical constraints when a new row is appended
 * Range:     u8 none, u16 n <= 6, u32 n <= 23, u64 n <= 60, u128 n <= 138,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* number of rows in the matrix */
    idx_t rows_cnt = n + 2;

    /* The sequence is defined for n ≥ 1, hence rows_cnt ≥ 3.
       The guard is kept only for completeness. */
    if (rows_cnt < 3) return 0;

    /* All 4‑bit rows that avoid 001 and 111 horizontally. */
    static const int rows[9] = {
        0b0000, 0b0100, 0b0101, 0b0110,
        0b1000, 0b1010, 0b1011, 0b1100, 0b1101
    };

    /* Transition validity: valid[i][j][k] is 1 if rows i, j, k
       can be consecutive without violating the vertical 001/111
       constraint.  The table is built once. */
    static int valid[9][9][9];
    static int init_done = 0;
    if (!init_done) {
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                for (int k = 0; k < 9; ++k) {
                    int ok = 1;
                    for (int col = 0; col < 4; ++col) {
                        int a = (rows[i] >> col) & 1;
                        int b = (rows[j] >> col) & 1;
                        int c = (rows[k] >> col) & 1;
                        if (a == 0 && b == 0 && c == 1) { ok = 0; break; }
                        if (a == 1 && b == 1 && c == 1) { ok = 0; break; }
                    }
                    valid[i][j][k] = ok;
                }
        init_done = 1;
    }

    /* DP over the last two rows.  dpPrev[p][q] counts the
       sequences that end with rows rows[p] and rows[q]. */
    seq_t dpPrev[9][9];
    seq_t dpNext[9][9];
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            dpPrev[i][j] = 1;          /* all first two rows admissible */

    /* Add rows 3 … rows_cnt through the transfer matrix. */
    for (idx_t step = 3; step <= rows_cnt; ++step) {
        /* zero dpNext */
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                dpNext[i][j] = 0;

        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                seq_t cur = dpPrev[i][j];
                if (!cur) continue;
                for (int k = 0; k < 9; ++k)
                    if (valid[i][j][k])
                        dpNext[j][k] += cur;
            }
        }

        /* prepare for next iteration */
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                dpPrev[i][j] = dpNext[i][j];
    }

    /* Sum all ending states to obtain the total number of matrices. */
    seq_t total = 0;
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            total += dpPrev[i][j];

    return total;
}
