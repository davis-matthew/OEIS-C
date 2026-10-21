/*
 * A056475  Number of palindromic structures using exactly six different
 *          symbols.
 * https://oeis.org/A056475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Stirling2( floor((n+1)/2), 6 )
 * Range:     u8 n <= 14, u16 n <= 20, u32 n <= 32, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* computes the number of palindromic structures using exactly six different symbols */
seq_t a(idx_t n)
{
    /* m = floored half‑size of the string */
    idx_t m = (n + 1) / 2;

    /* S[k] will hold the value of Stirling2(current_n, k) for k = 0..6 */
    seq_t S[7] = {0};
    S[0] = 1;                   /* S(0,0) = 1 */

    for (idx_t i = 1; i <= m; ++i) {
        /* update in reverse order to use previous row values */
        for (int k = 6; k >= 1; --k) {
            /* S(n,k) = k * S(n-1,k) + S(n-1,k-1) */
            S[k] = (seq_t)k * S[k] + S[k-1];
        }
        S[0] = 0;                /* for n > 0, Stirling2(n,0) = 0 */
    }
    return S[6];
}
