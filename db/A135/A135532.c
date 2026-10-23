/*
 * A135532  a(n) = 2*a(n-1) + a(n-2), with a(0)= -1, a(1)= 3.
 * https://oeis.org/A135532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = -1; a(1) = 3; a(n) = 2*a(n-1) + a(n-2) for n ≥ 2
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 24, i64 n <= 49, i128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two base cases explicitly. */
    if (n == 0) return (seq_t)-1;
    if (n == 1) return (seq_t)3;

    /* Iterate from the second term up to n. */
    seq_t a_prev2 = (seq_t)-1;   /* a(0) */
    seq_t a_prev1 = (seq_t)3;    /* a(1) */
    seq_t a_curr  = 0;

    /* Use idx_t for the loop counter to avoid signed/unsigned mix-ups. */
    idx_t i;
    for (i = 2; i <= n; ++i)
    {
        a_curr = (seq_t)2 * a_prev1 + a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_curr;
}
