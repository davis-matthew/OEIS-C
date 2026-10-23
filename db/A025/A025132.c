/*
 * A025132  a(n) = (1/2)*(3rd elementary symmetric function of C(n,0), C(n,1),
 *          ..., C(n,n)).
 * https://oeis.org/A025132
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2)*∑_{0≤i<j<k≤n} C(n,i)*C(n,j)*C(n,k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the third elementary symmetric sum of the binomial coefficients
   of order n, then divides by 2. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;           /* values for n < 2 are undefined */

    seq_t e1 = 0;   /* e1 = sum of the coefficients so far */
    seq_t e2 = 0;   /* e2 = sum of all pairwise products so far */
    seq_t e3 = 0;   /* e3 = sum of all triple products so far */

    seq_t x = 1;    /* C(n,0) = 1 */

    for (idx_t k = 0; k <= n; ++k) {
        /* Update the elementary symmetric sums with the new factor x */
        e3 += e2 * x;
        e2 += e1 * x;
        e1 += x;

        if (k == n) break;                 /* finished when we reach C(n,n) */

        /* Compute C(n, k+1) from C(n, k) */
        /* C(n, k+1) = C(n, k) * (n - k) / (k + 1) */
        x = x * (seq_t)(n - k) / (seq_t)(k + 1);
    }

    return e3 / 2;   /* a(n) = e3 / 2 */
}
