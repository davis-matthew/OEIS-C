/*
 * A224058  Number of nX2 0..3 arrays with rows, columns and antidiagonals
 *          unimodal and diagonals nondecreasing.
 * https://oeis.org/A224058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by dynamic programming over rows for 0..3
 *            entries in a 2‑column array, enforcing that each column is
 *            unimodal and that the main diagonal satisfies a_i ≤ b_{i+1}
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 21, u64 n <= 159, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of n×2 0..3 arrays with rows, columns and antidiagonals
   unimodal and diagonals nondecreasing. */
seq_t a(idx_t n)
{
    /* States: sA, sB ∈ {0,1} indicate whether the column has
       started decreasing (1) or not (0).  aVal, bVal ∈ {0..3}
       are the last values of the columns.  dp[sA][sB][aVal][bVal]
       counts arrays up to the current row.  Two 4‑dimensional
       arrays alternate for each new row. */
    static seq_t dp[2][2][4][4];
    static seq_t ndp[2][2][4][4];

    /* initialise for row 1 */
    for (int sA = 0; sA < 2; ++sA)
        for (int sB = 0; sB < 2; ++sB)
            for (int a = 0; a < 4; ++a)
                for (int b = 0; b < 4; ++b)
                    dp[sA][sB][a][b] = 0;
    for (int a = 0; a < 4; ++a)
        for (int b = 0; b < 4; ++b)
            dp[0][0][a][b] = 1;          /* first row, all values allowed */

    /* Special case n == 1 */
    if (n == 1) {
        seq_t result = 0;
        for (int sA = 0; sA < 2; ++sA)
            for (int sB = 0; sB < 2; ++sB)
                for (int a = 0; a < 4; ++a)
                    for (int b = 0; b < 4; ++b)
                        result += dp[sA][sB][a][b];
        return result;
    }

    /* iterate rows 2 .. n */
    for (idx_t row = 1; row < n; ++row) {
        /* clear ndp */
        for (int sA = 0; sA < 2; ++sA)
            for (int sB = 0; sB < 2; ++sB)
                for (int a = 0; a < 4; ++a)
                    for (int b = 0; b < 4; ++b)
                        ndp[sA][sB][a][b] = 0;

        /* extend each state to the next row */
        for (int sA = 0; sA < 2; ++sA)
            for (int sB = 0; sB < 2; ++sB)
                for (int a = 0; a < 4; ++a)
                    for (int b = 0; b < 4; ++b) {
                        seq_t cur = dp[sA][sB][a][b];
                        if (!cur) continue;
                        for (int na = 0; na < 4; ++na) {
                            int nsA;
                            if (sA == 0) {
                                nsA = (na < a) ? 1 : 0;
                            } else {          /* already decreasing */
                                if (na > a) continue;
                                nsA = 1;
                            }
                            for (int nb = 0; nb < 4; ++nb) {
                                /* diagonal constraint: a ≤ b' */
                                if (a > nb) continue;
                                int nsB;
                                if (sB == 0) {
                                    nsB = (nb < b) ? 1 : 0;
                                } else {
                                    if (nb > b) continue;
                                    nsB = 1;
                                }
                                ndp[nsA][nsB][na][nb] += cur;
                            }
                        }
                    }

        /* move ndp to dp for next iteration */
        for (int sA = 0; sA < 2; ++sA)
            for (int sB = 0; sB < 2; ++sB)
                for (int a = 0; a < 4; ++a)
                    for (int b = 0; b < 4; ++b)
                        dp[sA][sB][a][b] = ndp[sA][sB][a][b];
    }

    /* sum all final states */
    seq_t result = 0;
    for (int sA = 0; sA < 2; ++sA)
        for (int sB = 0; sB < 2; ++sB)
            for (int a = 0; a < 4; ++a)
                for (int b = 0; b < 4; ++b)
                    result += dp[sA][sB][a][b];

    return result;
}
