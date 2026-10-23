/*
 * A174755  x-values in the solution to x^2-47*y^2=1.
 * https://oeis.org/A174755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 48; a(n) = 96*a(n-1) - a(n-2) for n > 2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1; n==0 is unused */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)48;

    seq_t a_prev2 = (seq_t)1;  /* a(1) */
    seq_t a_prev1 = (seq_t)48; /* a(2) */
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t cur = (seq_t)96 * a_prev1 - a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = cur;
    }
    return a_prev1;
}
