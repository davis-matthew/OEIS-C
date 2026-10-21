/*
 * A085481  Duplicate of A030195.
 * https://oeis.org/A085481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2), a(1)=1, a(2)=3
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 13, i64 n <= 13, i128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    seq_t a_nm2 = (seq_t)1;  /* a(1) */
    seq_t a_nm1 = (seq_t)3;  /* a(2) */
    seq_t a_n   = 0;

    for (idx_t i = 3; i <= n; ++i) {
        a_n = (a_nm1 + a_nm2) * (seq_t)3;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
