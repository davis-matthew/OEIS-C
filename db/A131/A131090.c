/*
 * A131090  First differences of A131666.
 * https://oeis.org/A131090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A131666(n+1) - A131666(n). Recurrence: a(0) = 0, a(n+1) =
 *            2*a(n) + r(n), where r(n) is periodic with period 6: r = {+1,
 *            -2, +1, -1, +2, -1}
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Positive values of the periodic RHS, indices 0..5. */
    const unsigned int rhs[6] = {1, 2, 1, 1, 2, 1};

    seq_t cur = 0;                     /* current term a(i) ; starts with a(0) = 0 */
    for (idx_t i = 0; i < n; ++i)
    {
        idx_t k = i % 6;                /* period index */

        cur = cur + cur;                /* 2 * cur */

        if (k == 1 || k == 3 || k == 5) /* r(n) negative at positions 1,3,5 */
            cur -= (seq_t)rhs[k];
        else
            cur += (seq_t)rhs[k];
    }
    return cur;                         /* cur now holds a(n) */
}
