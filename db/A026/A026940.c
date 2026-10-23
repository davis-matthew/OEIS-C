/*
 * A026940  a(n) = Sum_{k=0..n-1} T(n,k) * T(n,k+1), with T given by A026300.
 * https://oeis.org/A026940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*n, 2*k+1) * binomial(2*k+1, k) /
 *            (k+2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return 0;
    /* initialize binomial coefficients */
    seq_t cn = (seq_t)(2 * n);   /* binom(2n, 1)          */
    seq_t dn = 1;                /* binom(1, 0)           */
    seq_t result = 0;

    for (idx_t k = 0; k <= n; ++k) {
        /* current term: cn * dn / (k+2) */
        seq_t term = cn * dn;
        term /= (seq_t)(k + 2);
        result += term;

        if (k == n)
            break;

        /* update cn to binom(2n, 2(k+1)+1) = binom(2n, 2k+3) */
        seq_t mult1 = (seq_t)(2 * n - (2 * k + 1));
        seq_t mult2 = (seq_t)(2 * n - (2 * k + 2));
        seq_t num   = cn * mult1 * mult2;
        seq_t den1  = (seq_t)((2 * k + 2) * (2 * k + 3));
        cn = num / den1;

        /* update dn to binom(2(k+1)+1, k+1) = binom(2k+3, k+1) */
        seq_t nmult1 = (seq_t)((2 * k + 2) * (2 * k + 3));
        seq_t nnum   = dn * nmult1;
        seq_t nden   = (seq_t)((k + 1) * (k + 2));
        dn = nnum / nden;
    }
    return result;
}
