/*
 * A174778  y-values in the solution to x^2 - 41*y^2 = 1.
 * https://oeis.org/A174778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4098*a(n-1) - a(n-2) with a(1)=0, a(2)=320
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)320;

    seq_t a_prev2 = (seq_t)0;   /* a(n-2) */
    seq_t a_prev1 = (seq_t)320; /* a(n-1) */
    seq_t a_curr  = a_prev1;    /* placeholder */

    /* Iteratively compute up to the requested index. */
    for (idx_t i = 3; i <= n; ++i) {
        a_curr = (seq_t)4098 * a_prev1 - a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_curr;
}
