/*
 * A240707  Sum of the middle parts in the partitions of 4n-1 into 3 parts.
 * https://oeis.org/A240707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{b=1}^{2n-1} b * min(b, 4n-1-2b) This is equivalent to
 *            the sum of the middle parts of all partitions of 4n-1 into three
 *            positive parts
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* m = 4n - 1 */
    seq_t m = (seq_t)4 * (seq_t)n - 1;
    /* upper bound for b is floor((m-1)/2) which equals 2n-1 */
    idx_t up = 2 * n - 1;

    for (idx_t b = 1; b <= up; ++b) {
        seq_t bseq = (seq_t)b;
        seq_t two_b = bseq + bseq;
        seq_t rem = m - two_b;          /* rem = m - 2b, always ≥ 1 in this loop */
        seq_t t = (rem < bseq) ? rem : bseq;  /* min(b, m-2b) */
        sum += bseq * t;
    }
    return sum;
}
