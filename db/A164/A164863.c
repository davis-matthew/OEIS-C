/*
 * A164863  Number of ways of placing n labeled balls into 9 indistinguishable
 *          boxes; word structures of length n using a 9-ary alphabet.
 * https://oeis.org/A164863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..9} stirling2(n,k)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(0)=1, a(1)=1, a(2)=2, ... (see OEIS A164863) */

seq_t a(idx_t n)
{
    /* Number of blocks we consider (9) */
    const int K = 9;
    seq_t S[10];
    /* Initialise all Stirling numbers to 0 */
    for (int k = 0; k <= K; ++k) {
        S[k] = 0;
    }
    /* Base: S(0,0)=1 */
    S[0] = 1;

    /* Iterate n times to build S(n,k) for k=0..9 */
    for (idx_t i = 1; i <= n; ++i) {
        /* Update from high k to low to use old values */
        for (int k = K; k >= 1; --k) {
            /* S(n,k) = k * S(n-1,k) + S(n-1,k-1) */
            S[k] = (seq_t)k * S[k] + S[k - 1];
        }
        /* For n>0, S(n,0)=0 */
        S[0] = 0;
    }

    /* Sum over k=0..9 to get the final term a(n) */
    seq_t sum = 0;
    for (int k = 0; k <= K; ++k) {
        sum += S[k];
    }
    return sum;
}
