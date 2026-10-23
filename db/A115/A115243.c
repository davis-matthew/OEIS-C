/*
 * A115243  Expansion of g.f. 2*x*(2*x + 1)/((4*x - 1)*(x - 1)*(x + 1)).
 * https://oeis.org/A115243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 3 + (-1)^n, with a(0) = 0
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;

    seq_t prev = (seq_t)0;
    seq_t cur;

    for (idx_t i = (idx_t)1; i <= n; ++i)
    {
        /* compute 4*a(n-1) + 3 + (-1)^i */
        cur = prev * (seq_t)4;
        cur += (seq_t)3;
        if ((i & (idx_t)1) == (idx_t)0)   /* even i → (-1)^i = +1 */
            cur += (seq_t)1;
        else                             /* odd i  → (-1)^i = -1 */
            cur -= (seq_t)1;

        prev = cur;
    }

    return prev;
}
