/*
 * A125905  a(0) = 1, a(1) = -4, a(n) = -4*a(n-1) - a(n-2) for n > 1.
 * https://oeis.org/A125905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0) = 1, a(1) = -4, a(n) = -4*a(n-1) - a(n-2) for n > 1
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 16, i64 n <= 33, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)(-4);

    /* Variables holding the two most recent values of the sequence. */
    seq_t a0 = (seq_t)1;   /* a(n-2) */
    seq_t a1 = (seq_t)(-4);/* a(n-1) */
    seq_t an;              /* a(n) */

    /* Iterate from 2 up to and including n, computing each term
       using the recurrence. */
    for (idx_t i = 2; i <= n; ++i) {
        an = - (seq_t)4 * a1 - a0; /* a(n) = -4*a(n-1) - a(n-2) */
        a0 = a1;                   /* shift for next iteration */
        a1 = an;
    }

    return an;
}
