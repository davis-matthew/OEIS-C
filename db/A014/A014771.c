/*
 * A014771  Squares of odd hexagonal numbers.
 * https://oeis.org/A014771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((2*n-1)*(4*n-3))^2 (OEIS A014771)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 91, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute the product (2*n-1)*(4*n-3) */
    seq_t prod = ((seq_t)2 * n - (seq_t)1) * ((seq_t)4 * n - (seq_t)3);
    /* square the product to get the final term */
    return prod * prod;
}
