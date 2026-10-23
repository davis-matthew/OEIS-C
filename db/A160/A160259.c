/*
 * A160259  Numerator of Hermite(n, 11/29).
 * https://oeis.org/A160259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0)=1, a(1)=22; for n>=2: a(n)=22*a(n-1)-2*(n-1)*841*a(n-2)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)22;

    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)22;  /* a(1) */

    for (idx_t i = 1; i < n; ++i) {
        seq_t coeff = (seq_t)2 * (seq_t)i * (seq_t)841; /* 2*i*841 */
        seq_t an = (seq_t)22 * a_nm1 - coeff * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = an;
    }
    return a_nm1;
}
