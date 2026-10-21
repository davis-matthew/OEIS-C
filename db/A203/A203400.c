/*
 * A203400  Partial sums of A050935.
 * https://oeis.org/A203400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) - a(n-3) + a(n-4); Initial values:
 *            a(1)=0, a(2)=0, a(3)=1, a(4)=2
 * Range:     i8 n <= 35, i16 n <= 75, i32 n <= 155, i64 n <= 312,
 *            i128 n <= 628, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)1;
    if (n == 4) return (seq_t)2;

    /* Keep the last four values: a(k-4), a(k-3), a(k-2), a(k-1). */
    seq_t a1 = (seq_t)0; /* a(k-4) */
    seq_t a2 = (seq_t)0; /* a(k-3) */
    seq_t a3 = (seq_t)1; /* a(k-2) */
    seq_t a4 = (seq_t)2; /* a(k-1) */

    for (idx_t k = 5; k <= n; ++k) {
        seq_t an = 2 * a4 - a3 - a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return a4;
}
