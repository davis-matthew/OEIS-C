/*
 * A161680  a(n) = binomial(n,2): number of size-2 subsets of {0,1,...,n} that
 *          contain no consecutive integers.
 * https://oeis.org/A161680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)/2
 * Range:     u8 n <= 23, u16 n <= 362, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 or 1 the result is 0. */
    if (n <= 1U) {
        return (seq_t)0;
    }
    /* Use the formula n*(n-1)/2.  Cast to seq_t before the
       arithmetic to keep type consistent and avoid
       integer promotion issues. */
    seq_t m = (seq_t)n;
    return m * (m - (seq_t)1) / (seq_t)2;
}
