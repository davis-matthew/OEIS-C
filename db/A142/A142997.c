/*
 * A142997  a(0) = 0, a(1) = 1, a(n+1) = (2*n^2+2*n+13)*a(n) - n^4*a(n-1).
 * https://oeis.org/A142997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0, a(1) = 1, a(n+1) = (2*n^2+2*n+13)*a(n) - n^4*a(n-1)
 *            (for n >= 0; a(n) is defined for n >= 0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t a_prev2 = 0;   /* a(0) */
    seq_t a_prev1 = 1;   /* a(1) */

    for (idx_t i = 1; i < n; ++i)   /* compute a(i+1) for i = 1 .. n-1 */
    {
        seq_t nn = (seq_t)i;                         /* i as seq_t */
        seq_t coeff = 2 * nn * nn + 2 * nn + 13;    /* (2*i^2+2*i+13) */
        seq_t term1 = coeff * a_prev1;

        seq_t nn4 = nn * nn * nn * nn;               /* i^4 */
        seq_t term2 = nn4 * a_prev2;

        seq_t a_curr = term1 - term2;

        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_prev1;   /* a(n) */
}
