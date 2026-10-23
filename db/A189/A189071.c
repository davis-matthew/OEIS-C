/*
 * A189071  The n-th derivative of x^10 evaluated at x=2.
 * https://oeis.org/A189071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2)*(11-n)*a(n-1), a(0) = 1024
 * Range:     u8 none, u16 n <= 2, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n > 10 the recurrence yields zero. */
    if (n > 10)
        return 0;
    seq_t res = 1024;
    for (idx_t i = 1; i <= n; ++i)
    {
        /* (11 - i) is always a small positive integer for i<=10. */
        res = (res * (seq_t)(11 - i)) / 2;
    }
    return res;
}
