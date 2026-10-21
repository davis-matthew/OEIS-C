/*
 * A076616  Number of permutations of {1,2,...,n} that result in a binary
 *          search tree (when elements of the permutation are inserted in that
 *          order) of height n-1 (i.e., the second largest possible height).
 * https://oeis.org/A076616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n ≤ 2, a(n) = 2^(n-3) * (n^2 – n – 4) for n > 2
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 25, u64 n <= 55, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2) return 0;

    /* (n^2 – n – 4) = n*(n-1) – 4 */
    seq_t base = ((seq_t)n * ((seq_t)n - 1)) - 4;

    /* compute 2^(n-3) by repeated doubling */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n - 3; ++i)
        pow2 *= 2;

    return base * pow2;
}
