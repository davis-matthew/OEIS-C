/*
 * A025133  4th elementary symmetric function of C(n,0), C(n,1), ..., C(n,n).
 * https://oeis.org/A025133
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4th elementary symmetric sum of the binomial coefficients
 *            C(n,0), C(n,1), …, C(n,n). That is, a(n) = Σ_{0≤i<j<k<l≤n}
 *            C(n,i)·C(n,j)·C(n,k)·C(n,l)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                     /* undefined for the offset; needs not be used */

    /* e[j] will hold the j‑th elementary symmetric sum of the
       binomial coefficients processed so far.  e[0] = 1 by definition.  */
    seq_t e[5] = { 0 };                /* indices 0..4 */
    e[0] = 1;

    seq_t c = 1;                       /* C(n,0) */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* Update the symmetric sums in reverse order so that
           the value for e[j-1] used on the right side is the old one.  */
        for (int j = 4; j >= 1; --j)
            e[j] = e[j] + e[j-1] * c;

        if (k == n)                    /* last binomial has been processed */
            break;

        /* Next binomial coefficient: C(n,k+1) = C(n,k) * (n-k) / (k+1)  */
        c = c * (seq_t)(n - k) / (seq_t)(k + 1);
    }

    return e[4];
}
