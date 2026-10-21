/*
 * A069721  Number of rooted unicursal planar maps with n edges and no
 *          vertices of valency 1 (unicursal means that exactly two vertices
 *          are of odd valency; there is an Eulerian path).
 * https://oeis.org/A069721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n-2) * (n-2) * binomial(2n-2, n-1) / n , n > 1 (a(1) =
 *            0, a(2) = 0)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset: a(1)=0, a(2)=0 */
    if (n <= 2)
        return (seq_t)0;

    /* Compute 2^(n-2) */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < n - 2; ++i)
        pow2 *= (seq_t)2;

    /* Compute the central binomial coefficient C(2n-2, n-1) using a multiplicative
       formula that keeps intermediate integers exact. */
    seq_t bin = (seq_t)1;          /* bin = 1 initially */
    idx_t upper = n - 1;           /* number of factors in the product */
    for (idx_t k = 1; k <= upper; ++k) {
        seq_t numerator   = (seq_t)(n - 1 + k); /* n-1 + k ranges from n to 2n-2 */
        seq_t denominator = (seq_t)k;
        bin = bin * numerator / denominator;   /* exact division */
    }

    /* Combine the remaining factors: (n-2)/n, then multiply by 2^(n-2) */
    seq_t temp = bin * (seq_t)(n - 2);
    temp /= (seq_t)n;          /* exact division */

    return temp * pow2;
}
