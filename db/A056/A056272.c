/*
 * A056272  Word structures of length n using a 5-ary alphabet.
 * https://oeis.org/A056272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..5} Stirling2(n,k)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 30, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset explicitly. */
    if (n == 0)
        return (seq_t)1;

    /* Up to k=5 the Stirling numbers are kept in an array. */
    seq_t S[6] = {0};
    seq_t Snew[6] = {0};
    S[0] = (seq_t)1;           /* S(0,0) = 1 */

    for (idx_t i = 1; i <= n; ++i) {
        Snew[0] = (seq_t)0;    /* S(i,0) = 0 for i>0 */
        for (int k = 1; k <= 5; ++k) {
            /* S(i,k) = S(i-1,k-1) + k * S(i-1,k) */
            Snew[k] = S[k-1] + (seq_t)k * S[k];
        }
        /* Move Snew to S for the next iteration. */
        for (int k = 0; k <= 5; ++k)
            S[k] = Snew[k];
    }

    /* Sum the entries to obtain a(n). */
    seq_t result = (seq_t)0;
    for (int k = 0; k <= 5; ++k)
        result += S[k];

    return result;
}
