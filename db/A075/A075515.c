/*
 * A075515  Fifth column of triangle A075498.
 * https://oeis.org/A075515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^n)*S2(n+5,5) (Stirling numbers of the second kind)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 3^n */
    seq_t power3 = 1;
    for (idx_t i = 1; i <= n; ++i)
        power3 *= 3;

    /* compute S(k,5) for k = 0 .. n+5 via recurrence
       S(k,m) = m*S(k-1,m) + S(k-1,m-1)  (0 <= m <= 5)   */
    seq_t S[6] = {0};
    S[0] = 1;                         /* S(0,0) = 1 */

    idx_t limit = n + 5;
    for (idx_t k = 1; k <= limit; ++k) {
        for (int m = 5; m >= 1; --m) {
            S[m] = (seq_t)m * S[m] + S[m-1];
        }
        /* for k >= 1, S(k,0) = 0 */
        S[0] = 0;
    }

    return power3 * S[5];
}
