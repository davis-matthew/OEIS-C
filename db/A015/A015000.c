/*
 * A015000  q-integers for q=-13.
 * https://oeis.org/A015000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(1) = 1, a(2) = -12; Recurrence: a(n) = -12*a(n-1) + 13*a(n-2)
 *            for n ≥ 3
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 9, i64 n <= 18, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* OEIS offset is 1; n < 1 is not used by the tests. */
    if (n <= 0) {
        return (seq_t)0;
    }
    if (n == 1) {
        return (seq_t)1;
    }
    if (n == 2) {
        return (seq_t)-12;
    }
    seq_t prevprev = (seq_t)1;   /* a(1) */
    seq_t prev     = (seq_t)-12; /* a(2) */
    seq_t cur;
    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)-12 * prev + (seq_t)13 * prevprev;
        prevprev = prev;
        prev = cur;
    }
    return cur;
}
