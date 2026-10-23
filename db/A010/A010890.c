/*
 * A010890  15th cyclotomic polynomial.
 * https://oeis.org/A010890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) is the coefficient of x^n in the 15th cyclotomic polynomial
 *            Φ₁₅(x), and a(n)=0 for n>8
 * Range:     i8 n <= 8, i16 n <= 8, i32 n <= 8, i64 n <= 8, i128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static seq_t coef[9];
    static int init = 0;
    if (!init) {
        /* Dividend: x^10 + x^5 + 1  (degree 10) */
        seq_t dividend[11] = {0};
        dividend[10] = 1;
        dividend[5]  = 1;
        dividend[0]  = 1;

        /* Denominator: x^2 + x + 1  (degree 2) */
        const seq_t denom[3] = {1, 1, 1};
        const int degD = 2;   /* degree of denom */
        const int degN = 10;  /* degree of dividend */

        seq_t quotient[9] = {0};
        for (int i = degN; i >= degD; --i) {
            seq_t q = dividend[i];          /* denom[degD]==1, no division needed */
            quotient[i - degD] = q;
            for (int j = 0; j <= degD; ++j) {
                dividend[i - degD + j] -= q * denom[j];
            }
        }

        for (int i = 0; i < 9; ++i)
            coef[i] = quotient[i];
        init = 1;
    }

    if (n < 9)
        return coef[n];
    return (seq_t)0;
}
