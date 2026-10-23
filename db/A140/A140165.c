/*
 * A140165  a(n) = -a(n-1) + 3*a(n-2), starting a(1)=1, a(2)=2.
 * https://oeis.org/A140165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) + 3*a(n-2), a(1)=1, a(2)=2
 * Range:     i8 n <= 8, i16 n <= 15, i32 n <= 28, i64 n <= 55, i128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    seq_t a_nm2 = (seq_t)1;  /* a(1) */
    seq_t a_nm1 = (seq_t)2;  /* a(2) */
    seq_t a_n;

    for (idx_t i = 3; i <= n; ++i) {
        a_n = -a_nm1 + (seq_t)3 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
