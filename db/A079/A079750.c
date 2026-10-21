/*
 * A079750  Operation count to create all permutations of n distinct elements
 *          using the "streamlined" version of Algorithm L (lexicographic
 *          permutation generation) from Knuth's The Art of Computer
 *          Programming, Vol. 4, chapter 7.2.1.2. Sequence gives number of
 *          comparisons required to find j in step L2.2'.
 * https://oeis.org/A079750
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * a(n-1) + n for n >= 4, a(3) = 0
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;          /* values below the offset are irrelevant */
    seq_t res = 0;               /* a(3) */
    for (idx_t i = 4; i <= n; ++i)
        res = (seq_t)i * res + (seq_t)i;
    return res;
}
