/*
 * A069008  Let M denote the 6 X 6 matrix with rows
 *          /1,1,1,1,1,1/1,1,1,1,1,0/1,1,1,1,0,0/1,1,1,0,0,0/1,1,0,0,0,0/1,0,0,0,0,0/
 *          and A(n) the vector (x(n),y(n),z(n),t(n),u(n),v(n)) = M^n*A where
 *          A is the vector (1,1,1,1,1,1); then a(n) = z(n).
 * https://oeis.org/A069008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum of the third row of M^n, where M is the 6×6 matrix [1
 *            1 1 1 1 1; 1 1 1 1 1 0; 1 1 1 1 0 0; 1 1 1 0 0 0; 1 1 0 0 0 0; 1
 *            0 0 0 0 0]. a(0)=1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base matrix M */
    static const seq_t M0[6][6] = {
        {1,1,1,1,1,1},
        {1,1,1,1,1,0},
        {1,1,1,1,0,0},
        {1,1,1,0,0,0},
        {1,1,0,0,0,0},
        {1,0,0,0,0,0}
    };

    /* current result matrix R (initially identity) */
    seq_t R[6][6] = {0};
    for (int i = 0; i < 6; ++i) R[i][i] = 1;

    /* working matrix P = M0 */
    seq_t P[6][6];
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 6; ++j)
            P[i][j] = M0[i][j];

    idx_t exp = n;
    while (exp) {
        if (exp & 1) {
            seq_t T[6][6] = {0};
            for (int i = 0; i < 6; ++i)
                for (int j = 0; j < 6; ++j) {
                    seq_t sum = 0;
                    for (int k = 0; k < 6; ++k)
                        sum += R[i][k] * P[k][j];
                    T[i][j] = sum;
                }
            for (int i = 0; i < 6; ++i)
                for (int j = 0; j < 6; ++j)
                    R[i][j] = T[i][j];
        }
        exp >>= 1;
        if (!exp) break;
        seq_t T[6][6] = {0};
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j < 6; ++j) {
                seq_t sum = 0;
                for (int k = 0; k < 6; ++k)
                    sum += P[i][k] * P[k][j];
                T[i][j] = sum;
            }
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j < 6; ++j)
                P[i][j] = T[i][j];
    }

    /* a(n) is the sum of the third row of R (index 2) */
    seq_t res = 0;
    for (int j = 0; j < 6; ++j)
        res += R[2][j];
    return res;
}
