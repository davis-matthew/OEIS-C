/*
 * A079756  Operation count to create all permutations of n distinct elements
 *          using the "streamlined" version of Algorithm L (lexicographic
 *          permutation generation) from Knuth's The Art of Computer
 *          Programming, Vol. 4, chapter 7.2.1.2. Sequence gives number of
 *          interchanges in reversal step.
 * https://oeis.org/A079756
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + (n-1)*(floor((n-1)/2)-1) for n≥5 Initial
 *            values: a(3)=0, a(4)=0 For n<5 the value is 0 (indexing starts
 *            at n=3)
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* return 0 for indices below the offset (3) and for n=4 */
    if (n <= 4) return 0;

    /* a(4) = 0 */
    seq_t prev = 0;

    /* iterate from 5 up to n, each step uses only the previous term */
    for (idx_t i = 5; i <= n; ++i)
    {
        /* term = n * a(n-1) */
        seq_t term = (seq_t)i * prev;

        /* add = (n-1)*(floor((n-1)/2)-1) */
        idx_t t = (i - 1) / 2;           /* floor((n-1)/2) */
        seq_t add = (seq_t)(i - 1) * (seq_t)(t - 1);

        prev = term + add;
    }
    return prev;
}
