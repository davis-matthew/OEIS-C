/*
 * A158534  a(n) = Hermite(n,10).
 * https://oeis.org/A158534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Hermite(n,10) a(0) = 1, a(1) = 20, for n >= 2: a(n) =
 *            20*a(n-1) - 2*(n-1)*a(n-2) (physicist Hermite recurrence)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 7, i64 n <= 14, i128 n <= 30,
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
        return (seq_t)20;

    /* constants */
    const seq_t two  = (seq_t)2;
    const seq_t ten  = (seq_t)10;
    const seq_t two_times_ten = two * ten;   /* 20 */

    /* iterate recurrence */
    seq_t H_nm2 = (seq_t)1;   /* a(0) */
    seq_t H_nm1 = (seq_t)20;  /* a(1) */
    seq_t H_n   = H_nm1;      /* placeholder */

    for (idx_t k = 2; k <= n; ++k) {
        /* a(k) = 20*a(k-1) - 2*(k-1)*a(k-2) */
        H_n = two_times_ten * H_nm1 - two * (seq_t)(k - 1) * H_nm2;
        H_nm2 = H_nm1;
        H_nm1 = H_n;
    }
    return H_nm1;
}
