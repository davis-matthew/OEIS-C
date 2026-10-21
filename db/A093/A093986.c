/*
 * A093986  a(1) = 1, a(2) = 1, a(n+1) = 2n*a(n) - a(n-1). Symmetrically a(n)
 *          = (a(n-1) + a(n+1))/((n-1) + (n+1)).
 * https://oeis.org/A093986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 1, a(n+1) = 2n * a(n) - a(n-1) (n ≥ 2)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two known base cases */
    if (n == 1 || n == 2)
        return (seq_t)1;

    /* a0  = a(n-2),  a1 = a(n-1) */
    seq_t a0 = (seq_t)1; /* a(1) */
    seq_t a1 = (seq_t)1; /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i)
    {
        /* recurrence: a(i) = 2*(i-1)*a(i-1) - a(i-2) */
        seq_t factor = (seq_t)(2 * (i - 1));
        cur = factor * a1 - a0;
        a0 = a1;
        a1 = cur;
    }

    return a1;
}
