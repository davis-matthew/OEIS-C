/*
 * A140167  a(n) = (-1)*a(n-1) + 3*a(n-2) with a(1)=-1 and a(2)=1.
 * https://oeis.org/A140167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) + 3*a(n-2), a(1) = -1, a(2) = 1
 * Range:     i8 n <= 7, i16 n <= 14, i32 n <= 27, i64 n <= 53, i128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 1) return (seq_t)-1;
    if (n == 2) return (seq_t)1;

    /* a_prev2 = a(n-2), a_prev1 = a(n-1). Start with a(1), a(2). */
    seq_t a_prev2 = (seq_t)-1;   /* a(1) */
    seq_t a_prev1 = (seq_t) 1;   /* a(2) */
    seq_t a_curr = 0;

    /* Iterate from n = 3 up to the requested n. */
    for (idx_t i = 3; i <= n; ++i) {
        a_curr = -a_prev1 + (seq_t)3 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_curr;
}
