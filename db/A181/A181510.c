/*
 * A181510  Number of permutations of the multiset {1,1,2,2,3,3,...,n+1,n+1}
 *          avoiding the permutation patterns {132, 231, 2134}.
 * https://oeis.org/A181510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^2 + 6*n - 2
 * Range:     u8 n <= 9, u16 n <= 179, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t before arithmetic */
    seq_t nn = (seq_t)n;
    /* Compute the polynomial exactly in seq_t */
    return 2*nn*nn + 6*nn - 2;
}
