/*
 * A049434  Stirling numbers of second kind: 8th column of Stirling2 triangle
 *          A008277.
 * https://oeis.org/A049434
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = S(n,8) (Stirling numbers of the second kind)
 * Range:     u8 n <= 9, u16 n <= 11, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 8)
        return (seq_t)0;                       /* values below the offset are irrelevant */

    seq_t st[9] = {0};                        /* st[k] holds S(current_n, k) */
    st[0] = 1;                                 /* S(0,0) = 1 */

    for (idx_t i = 1; i <= n; ++i)
    {
        int maxk = (i < 8) ? (int)i : 8;       /* k cannot exceed i or 8 */
        for (int k = maxk; k >= 1; --k)
        {
            st[k] = st[k - 1] + (seq_t)k * st[k];
        }
        st[0] = 0;                             /* S(i,0) = 0 for i > 0 */
    }

    return st[8];                              /* S(n,8) */
}
