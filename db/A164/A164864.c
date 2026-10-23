/*
 * A164864  Number of ways of placing n labeled balls into 10
 *          indistinguishable boxes; word structures of length n using a
 *          10-ary alphabet.
 * https://oeis.org/A164864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..10} Stirling2(n,k)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Maximum number of boxes (k) */
    static const int MAX_K = 10;
    /* Stirling numbers S(k) for current n */
    seq_t S[MAX_K + 1];

    /* initialise all entries to 0 */
    for (int k = 0; k <= MAX_K; ++k)
        S[k] = 0;
    S[0] = 1;                      /* S(0,0) = 1 */

    if (n == 0)
        return S[0];

    for (idx_t i = 1; i <= n; ++i) {
        for (int k = MAX_K; k >= 1; --k) {
            /* S(n,k) = S(n-1,k-1) + k * S(n-1,k) */
            S[k] = S[k - 1] + (seq_t)k * S[k];
        }
        S[0] = 0;                  /* S(n,0) = 0 for n > 0 */
    }

    seq_t result = 0;
    for (int k = 0; k <= MAX_K; ++k)
        result += S[k];
    return result;
}
