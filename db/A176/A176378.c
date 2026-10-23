/*
 * A176378  y-values in the solution to x^2-70*y^2=1.
 * https://oeis.org/A176378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 502*a(n-1) - a(n-2), a(1) = 0, a(2) = 30
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* a(1) = 0 */
    if (n == 2) return 30;         /* a(2) = 30 */

    seq_t prev2 = 0;               /* a(1) */
    seq_t prev1 = 30;              /* a(2) */
    seq_t curr = 0;

    for (idx_t i = 3; i <= n; ++i)
    {
        curr = (seq_t)502 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
