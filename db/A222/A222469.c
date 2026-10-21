/*
 * A222469  Denominator sequence of the n-th convergent of the continued
 *          fraction 1/(1 - 2/(2 - 2/(3 - 2/(4 - ...)))).
 * https://oeis.org/A222469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, frac
 * Formula:   a(n) = n*a(n-1) - 2*a(n-2), with a(-1)=0, a(0)=1, valid for n >=
 *            0
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 20, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A222469: denominator of the n-th
   convergent of the continued fraction 1/(1-2/(2-2/(3-2/(4-...)))).
   The recurrence is a(n) = n*a(n-1) - 2*a(n-2). */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* a(0) = 1, a(1) = 1 */
    seq_t a_prevprev = (seq_t)1; /* a(0) */
    seq_t a_prev     = (seq_t)1; /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t temp = (seq_t)i * a_prev - (seq_t)2 * a_prevprev;
        a_prevprev = a_prev;
        a_prev     = temp;
    }
    return a_prev;
}
