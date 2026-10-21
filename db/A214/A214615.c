/*
 * A214615  Row sums of A060338.
 * https://oeis.org/A214615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) - a(n-1) + (n-1)^2 * a(n-2) = 0 Initial terms: a(0) = 1,
 *            a(1) = 1
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special cases for the first two terms. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* We use only two temporary variables; the recurrence only depends
       on a(n-1) and a(n-2). */
    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)1;   /* a(1) */
    seq_t a_n, k, factor;

    /* Iterate from i = 2 up to n, computing a(i). */
    for (idx_t i = 2; i <= n; ++i) {
        k = (seq_t)(i - 1);          /* (i-1) as seq_t */
        factor = k * k;              /* (i-1)^2 */
        a_n = a_nm1 - factor * a_nm2;/* a(i) = a(i-1) - (i-1)^2 * a(i-2) */

        /* Shift the window for the next iteration. */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
