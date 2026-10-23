/*
 * A152620  a(n)=-8*a(n-1)-6*a(n-2), n>1 ; a(0)=1, a(1)=-2 .
 * https://oeis.org/A152620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -8*a(n-1) - 6*a(n-2), a(0)=1, a(1)=-2
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 11, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    if (n == 1)
        return -2;

    seq_t prev2 = 1;   /* a(0) */
    seq_t prev1 = -2;  /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = -8 * prev1 - 6 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}
