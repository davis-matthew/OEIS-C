/*
 * A159449  Numerator of Hermite(n, 6/11).
 * https://oeis.org/A159449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 12*a(n-1) - 242*(n-1)*a(n-2), a(0)=1, a(1)=12
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 6, i64 n <= 12, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)12;

    seq_t prev = (seq_t)1;   /* a(0) */
    seq_t curr = (seq_t)12;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t term1 = (seq_t)12 * curr;
        seq_t term2 = (seq_t)242 * (i - 1) * prev;
        seq_t next  = term1 - term2;
        prev = curr;
        curr = next;
    }
    return curr;
}
