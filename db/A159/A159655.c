/*
 * A159655  Numerator of Hermite(n, 17/19).
 * https://oeis.org/A159655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 34*A(n-1) - 722*(n-1)*A(n-2), a(0)=1, a(1)=34
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)34;

    seq_t a_nm2 = (seq_t)1;   /* A(n-2) */
    seq_t a_nm1 = (seq_t)34;  /* A(n-1) */
    seq_t a_n   = 0;          /* current A(n) */

    for (idx_t i = 2; i <= n; ++i) {
        /* coefficient 722*(i-1) */
        seq_t coeff = (seq_t)722 * ((seq_t)i - 1);
        a_n = a_nm1 * (seq_t)34 - coeff * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
