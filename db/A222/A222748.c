/*
 * A222748  Poly-Cauchy numbers c_n^(-4).
 * https://oeis.org/A222748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} Stirling1(n,k)*(k+1)^4
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 13, i64 n <= 21, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 the value is 1 */
    if (n == 0) return (seq_t)1;

    /* M_m(n) = sum_{k=0..n} Stirling1(n,k)*(k+1)^m  (m = 0..4) */
    seq_t M[5], newM[5];
    for (int i = 0; i < 5; ++i) M[i] = (seq_t)1;  /* M_m(0) = 1 for all m */

    /* Binomial coefficients C(m,r) for m,r ≤ 4 */
    const int comb[5][5] = {
        {1, 0, 0, 0, 0},
        {1, 1, 0, 0, 0},
        {1, 2, 1, 0, 0},
        {1, 3, 3, 1, 0},
        {1, 4, 6, 4, 1}
    };

    for (idx_t cur = 0; cur < n; ++cur) {          /* cur = current n (before increment) */
        seq_t cur_s = (seq_t)cur;
        for (int mIdx = 0; mIdx < 5; ++mIdx) {
            seq_t sum = 0;
            for (int r = 0; r <= mIdx; ++r) {
                sum += (seq_t)comb[mIdx][r] * M[r];
            }
            newM[mIdx] = sum - cur_s * M[mIdx];
        }
        for (int i = 0; i < 5; ++i) M[i] = newM[i];
    }

    return M[4];   /* a(n) = M_4(n) */
}
