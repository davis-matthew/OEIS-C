/*
 * A084222  a(n) = -2*a(n-1) + 3*a(n-2), with a(0)=1, a(1)=2.
 * https://oeis.org/A084222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -2*a(n-1) + 3*a(n-2), a(0) = 1, a(1) = 2
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 20, i64 n <= 41, i128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    seq_t a0 = (seq_t)1;  /* a(0) */
    seq_t a1 = (seq_t)2;  /* a(1) */
    seq_t an = a0;        /* placeholder */
    for (idx_t i = 2; i <= n; ++i) {
        an = a0 * (seq_t)3 - a1 * (seq_t)2;  /* a(i) = 3*a(i-2) - 2*a(i-1) */
        a0 = a1;
        a1 = an;
    }
    return an;
}
