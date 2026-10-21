/*
 * A090592  (1,1) entry of powers of the orthogonal design shown below.
 * https://oeis.org/A090592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) - 7*a(n-2), a(1) = 1, a(2) = -5
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 22, i64 n <= 44, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0)        /* undefined for the sequence; return 0 as a safe default */
        return 0;
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)(-5);

    /* Iteratively compute the recurrence. */
    seq_t a_prev2 = (seq_t)1;          /* a(1) */
    seq_t a_prev1 = (seq_t)(-5);       /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)2 * a_prev1 - (seq_t)7 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = cur;
    }
    return cur;
}
