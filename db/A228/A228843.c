/*
 * A228843  a(n) = 4^n*A228842(n).
 * https://oeis.org/A228843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=2, a(1)=24; a(n)=24*a(n-1)-64*a(n-2) for n>1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)24;

    seq_t a_prev2 = (seq_t)2;  /* a(0) */
    seq_t a_prev1 = (seq_t)24; /* a(1) */
    seq_t a_curr = 0;

    /* Compute iteratively up to n */
    for (idx_t i = 2; i <= n; ++i)
    {
        a_curr = (seq_t)24 * a_prev1 - (seq_t)64 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_curr;
}
