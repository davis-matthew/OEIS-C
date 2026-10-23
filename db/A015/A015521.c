/*
 * A015521  a(n) = 3*a(n-1) + 4*a(n-2), a(0) = 0, a(1) = 1.
 * https://oeis.org/A015521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two initial values directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Compute the terms iteratively, storing only the two
       previous values to keep memory usage minimal. */
    seq_t a_prev2 = (seq_t)0;      /* a(0) */
    seq_t a_prev1 = (seq_t)1;      /* a(1) */
    seq_t a_cur;

    for (idx_t i = 2; i <= n; ++i) {
        a_cur  = a_prev1 * (seq_t)3 + a_prev2 * (seq_t)4;
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }
    return a_prev1;
}
