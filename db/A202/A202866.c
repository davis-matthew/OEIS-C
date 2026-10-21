/*
 * A202866  Number of 4 X 4 0..n arrays with row and column sums one greater
 *          than the previous row and column.
 * https://oeis.org/A202866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2789*n^10 + 27890*n^9 + 100095*n^8 + 131400*n^7 * -
 *            35133*n^6 - 195090*n^5 - 71795*n^4 + 104200*n^3 * + 41844*n^2 -
 *            30600*n) / 37800 * * This is an integer polynomial giving the
 *            number of 4×4 arrays with * entries 0..n whose row and column
 *            sums increase by one from the * previous row/colu...
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* offset is 1; value for n=0 is unused */

    /* coefficients multiplied by 37800 (the common denominator) */
    static const long long coeff[11] = {
        2789,      /* 2789/37800  * n^10 */
        27890,     /* 2789/3780   * n^9  */
        100095,    /* 6673/2520   * n^8  */
        131400,    /* 73/21      * n^7  */
        -35133,    /* -1673/1800 * n^6  */
        -195090,   /* -929/180   * n^5  */
        -71795,    /* -14359/7560 * n^4 */
        104200,    /* 521/189    * n^3  */
        41844,     /* 3487/3150  * n^2  */
        -30600,    /* -17/21     * n^1  */
        0          /* constant term */
    };
    const unsigned LIM = 37800;       /* common denominator */

    seq_t res = (seq_t)coeff[0];       /* start with the leading term */

    for (int i = 1; i < 11; ++i) {
        res = res * n;                 /* multiply by n */
        if (coeff[i] >= 0)
            res += (seq_t)coeff[i];
        else
            res -= (seq_t)(-coeff[i]); /* subtract absolute value */
    }

    return res / (seq_t)LIM;           /* exact division, result fits in seq_t */
}
