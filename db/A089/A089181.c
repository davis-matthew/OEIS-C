/*
 * A089181  (1,3) entry of powers of the orthogonal design shown in A090592.
 * https://oeis.org/A089181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*a(n-1) - 7*a(n-2), a(1)=1, a(2)=2
 * Range:     i8 n <= 6, i16 n <= 11, i32 n <= 24, i64 n <= 45, i128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle base cases directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    seq_t prev2 = (seq_t)1;      /* a(n-2) */
    seq_t prev1 = (seq_t)2;      /* a(n-1) */
    seq_t cur;

    /* iterate from 3 up to n */
    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)2 * prev1 - (seq_t)7 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
