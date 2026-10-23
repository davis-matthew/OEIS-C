/*
 * A047175  Number of nonempty subsets of {1,2,...,n} in which exactly 3/4 of
 *          the elements are <= (n-1)/2.
 * https://oeis.org/A047175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k>=1, 3k<=floor((n-1)/2), k<=n-floor((n-1)/2)}
 *            binom(floor((n-1)/2),3k) * binom(n-floor((n-1)/2),k)
 * Range:     u8 n <= 14, u16 n <= 24, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* L = floor((n-1)/2), H = n - L = floor(n/2)+1   */
    idx_t L = (n - 1) / 2;
    idx_t H = n - L;

    /* Only n with L >= 3 can have a non‑zero value.  */
    if (L < 3) return 0;

    /* Maximum k allowed by the two inequalities.   */
    idx_t maxk = L / 3;
    if (maxk > H) maxk = H;
    if (maxk == 0) return 0;

    /* Iteratively build the binomial coefficients for
       C(L,i) and C(H,k).  No dynamic allocation is needed. */
    seq_t combL = 1;      /* C(L,0) = 1 */
    seq_t combH = 1;      /* C(H,0) = 1 */
    seq_t result = 0;

    for (idx_t i = 1, k = 0; i <= 3 * maxk; ++i)
    {
        /* Update C(L,i) from C(L,i-1) */
        combL = combL * (L - i + 1) / i;

        if (i % 3 == 0)   /* i == 3*k */
        {
            ++k;                       /* k = i/3 */
            /* Update C(H,k) from C(H,k-1) */
            combH = combH * (H - k + 1) / k;

            /* Add the contribution for this k */
            result += combL * combH;
        }
    }
    return result;
}
