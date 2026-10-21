/*
 * A076007  Seventh column of triangle A075503.
 * https://oeis.org/A076007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8^n) * S2(n+7, 7) where S2(m,k) are Stirling numbers of
 *            the second kind
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 8^n */
    seq_t pow8 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow8 *= (seq_t)8;

    /* compute S2(n+7, 7) by dynamic programming */
    seq_t S[8] = {0};        /* S[k] holds S2(m, k) for current m   */
    S[0] = 1;                /* S2(0,0) = 1 */

    idx_t target = n + 7;     /* final m value */

    for (idx_t m = 1; m <= target; ++m)
    {
        seq_t newS[8] = {0};
        int maxk = (m < 7) ? (int)m : 7;

        for (int k = 1; k <= maxk; ++k)
            newS[k] = (seq_t)k * S[k] + S[k - 1];

        /* copy back only up to maxk */
        for (int k = 0; k <= maxk; ++k)
            S[k] = newS[k];
    }

    /* final answer */
    return S[7] * pow8;
}
