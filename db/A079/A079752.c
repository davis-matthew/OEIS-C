/*
 * A079752  Operation count to create all permutations of n distinct elements
 *          using the "streamlined" version of Algorithm L (lexicographic
 *          permutation generation) from Knuth's The Art of Computer
 *          Programming, Vol. 4, chapter 7.2.1.2. Sequence gives number of
 *          times the search for an element exchangeable with...
 * https://oeis.org/A079752
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * a(n-1) + n - 2 , a(3) = 0 (n ≥ 4)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;        /* behaviour below offset irrelevant */
    seq_t val = 0;                     /* a(3) */
    if (n == 3) return val;
    for (idx_t i = 4; i <= n; ++i) {
        val = (seq_t)i * val + (seq_t)i - 2;
    }
    return val;
}
