/*
 * A160362  Numerator of Hermite(n, 3/32).
 * https://oeis.org/A160362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0)=1, a(1)=3; a(n) = 3*a(n-1) - 512*(n-1)*a(n-2) for n >= 2
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the numerator of Hermite(n, 3/32) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)3;

    seq_t a_nm2 = (seq_t)1;      /* a(0) */
    seq_t a_nm1 = (seq_t)3;      /* a(1) */
    seq_t a_next;
    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = 3*a(i-1) - 512*(i-1)*a(i-2) */
        a_next = (seq_t)3 * a_nm1
                 - (seq_t)512 * (seq_t)(i - 1) * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_next;
    }
    return a_nm1;
}
