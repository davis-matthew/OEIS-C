/*
 * A024389  [ (4th elementary symmetric function of S(n))/(2nd elementary
 *          symmetric function of S(n)) ], where S(n) = {first n+3 positive
 *          integers congruent to 1 mod 4}.
 * https://oeis.org/A024389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( e4(S(n)) / e2(S(n)) ) where S(n) = {1 + 4k | k =
 *            0..n+2}
 * Range:     u8 none, u16 n <= 1, u32 n <= 12, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n + 3;                    /* length of the set S(n) */

    /* Power sums S_j = sum_{t=0}^{m-1} t^j */
    seq_t S[5];
    S[0] = m;
    S[1] = m * (m - (seq_t)1) / 2;
    S[2] = (m - (seq_t)1) * m * ((seq_t)2 * m - (seq_t)1) / 6;
    seq_t tmp = m * (m - (seq_t)1) / 2;
    S[3] = tmp * tmp;
    seq_t t1 = m - (seq_t)1;
    seq_t t2 = (seq_t)2 * m - (seq_t)1;
    seq_t t3 = (seq_t)3 * m * m - (seq_t)3 * m - (seq_t)1;
    S[4] = t1 * m * t2 * t3 / 30;

    /* Powers of the difference d = 4 */
    seq_t d_pow[5];
    d_pow[0] = (seq_t)1;
    d_pow[1] = (seq_t)4;
    d_pow[2] = (seq_t)16;
    d_pow[3] = (seq_t)64;
    d_pow[4] = (seq_t)256;

    /* Binomial coefficients C(k, j) for k = 0..4 */
    const int binom[5][5] = {
        {1},
        {1, 1},
        {1, 2, 1},
        {1, 3, 3, 1},
        {1, 4, 6, 4, 1}
    };

    /* Power sums P_k of the terms of S(n) */
    seq_t P[5];
    P[0] = m;                 /* unused but convenient */
    for (idx_t k = 1; k <= 4; ++k) {
        seq_t sum = 0;
        for (idx_t j = 0; j <= k; ++j) {
            sum += (seq_t)binom[k][j] * d_pow[j] * S[j];
        }
        P[k] = sum;
    }

    /* Elementary symmetric sums */
    seq_t e1 = P[1];
    seq_t e2 = (e1 * e1 - P[2]) / 2;
    seq_t e3 = (e2 * e1 - e1 * P[2] + P[3]) / 3;
    seq_t e4 = (e3 * P[1] - e2 * P[2] + e1 * P[3] - P[4]) / 4;

    /* Final result: floor(e4 / e2) */
    return e4 / e2;
}
