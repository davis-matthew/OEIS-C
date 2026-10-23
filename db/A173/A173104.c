/*
 * A173104  The number of possible borders of Latin squares.
 * https://oeis.org/A173104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! (n−2)! [(n−1)/(n−2) d[n−1]^2 + 2 d[n−1] d[n−2] +
 *            (2n−5)/(n−3) d[n−2]^2] for n>3, where d[k] is the derangement of
 *            k (A000166); base cases: a(1)=1, a(2)=2, a(3)=12
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)12;

    seq_t fact = (seq_t)1;
    seq_t fact_n_minus_2 = (seq_t)1;
    seq_t der_prev2 = (seq_t)1;  /* d[0] */
    seq_t der_prev1 = (seq_t)0;  /* d[1] */

    for (idx_t i = 1; i <= n; ++i) {
        fact *= (seq_t)i;
        if (i <= n - 2) fact_n_minus_2 *= (seq_t)i;
        if (i >= 2 && i <= n - 1) {
            seq_t der_i = (seq_t)(i - 1) * (der_prev1 + der_prev2);
            der_prev2 = der_prev1;
            der_prev1 = der_i;
        }
    }

    seq_t d_n1 = der_prev1;  /* d[n−1] */
    seq_t d_n2 = der_prev2;  /* d[n−2] */
    seq_t big_n = (seq_t)n;

    seq_t term1 = (big_n - (seq_t)1) * d_n1 * d_n1;
    term1 /= (big_n - (seq_t)2);

    seq_t term2 = (seq_t)2 * d_n1 * d_n2;

    seq_t term3 = ((big_n * (seq_t)2 - (seq_t)5) * d_n2 * d_n2);
    term3 /= (big_n - (seq_t)3);

    seq_t sum = term1 + term2 + term3;

    return fact * fact_n_minus_2 * sum;
}
