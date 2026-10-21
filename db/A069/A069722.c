/*
 * A069722  Number of rooted unicursal planar maps with n edges and exactly
 *          one vertex of valency 1 (unicursal means that exactly two vertices
 *          are of odd valency; there is an Eulerian path).
 * https://oeis.org/A069722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n-1) * binomial(2n-2, n-1), a(1) = 0
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset 1: a(1) = 0 */
    if (n == 1)
        return (seq_t)0;

    /* compute 2^(n-1) by successive doubling */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 1; i <= n - 1; ++i)
        pow2 = pow2 + pow2;          /* pow2 = 2*pow2 */

    /* compute binomial(2n-2, n-1) multiplicatively */
    idx_t m = 2 * n - 2;             /* upper part of binomial */
    idx_t k = n - 1;                 /* lower part of binomial */
    if (k > m - k)                  /* use symmetry */
        k = m - k;
    seq_t bin = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        /* bin = bin * (m - k + i) / i;  exact division at each step */
        bin = (bin * (seq_t)(m - k + i)) / (seq_t)i;
    }

    /* final result */
    return pow2 * bin;
}
