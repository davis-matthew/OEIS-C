/*
 * A133132  Number of surjections from an n-element set to a ten-element set.
 * https://oeis.org/A133132
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10! * S(n,10) where the Stirling numbers of the second
 *            kind satisfy S(i,k) = k*S(i-1,k) + S(i-1,k-1) (k ≥ 1), S(i,0)=0
 *            for i>0 and S(0,0)=1
 * Range:     u8 none, u16 none, u32 n <= 11, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* for n < 10 the number of onto functions is zero */
    if (n < 10)
        return (seq_t)0;

    /* S(k) holds the current values for S(current_i, k)   */
    seq_t S[11];
    for (int k = 0; k <= 10; ++k)
        S[k] = (seq_t)0;
    S[0] = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i) {
        /* we only need k up to 10, but not larger than i  */
        int lastk = (i < 10) ? (int)i : 10;
        for (int k = lastk; k >= 1; --k) {
            /* S[i][k] = k * S[i-1][k] + S[i-1][k-1]  */
            S[k] = (seq_t)k * S[k] + S[k-1];
        }
        S[0] = (seq_t)0;            /* S[i][0] = 0 for i > 0 */
    }

    /* Multiply by 10! = 3628800 */
    const seq_t FACT10 = 3628800u;
    return S[10] * FACT10;
}
