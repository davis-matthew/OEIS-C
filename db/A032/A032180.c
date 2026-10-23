/*
 * A032180  Number of ways to partition n labeled elements into 6 pie slices.
 * https://oeis.org/A032180
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 120 * S(n,6), where S(n,k) satisfies the recurrence
 *            S(n,k) = k*S(n-1,k) + S(n-1,k-1) with S(0,0)=1 and S(0,k>0)=0.
 *            The function evaluates this directly for k up to 6
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n below the offset the behavior is unspecified; return 0. */
    if (n < 6)
        return 0;

    /* Stirling numbers of the second kind for k = 0..6.
       We keep only the current row; the recurrence is applied iteratively. */
    seq_t S[7] = {0};          /* S(k) holds S(current_n, k) */
    S[0] = (seq_t)1;           /* S(0,0) = 1 */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* Update from high k to low to use old values for S(k-1). */
        for (int k = 6; k >= 1; --k)
        {
            seq_t term1 = S[k] * (seq_t)k;   /* k * S(n-1, k) */
            seq_t term2 = S[k - 1];          /* S(n-1, k-1) */
            S[k] = term1 + term2;
        }
        S[0] = (seq_t)0; /* S(n,0) = 0 for n > 0 */
    }

    /* a(n) = 120 * S(n,6) */
    return S[6] * (seq_t)120;
}
