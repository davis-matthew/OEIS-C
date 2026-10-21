/*
 * A236364  Sum of all the middle parts in the partitions of 3n into 3 parts.
 * https://oeis.org/A236364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(3-3*(-1)^n+10*n^2)/16
 * Range:     u8 n <= 2, u16 n <= 18, u32 n <= 770, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t nseq = (seq_t)n;
    seq_t n2   = nseq * nseq;          /* n^2 */
    seq_t expr = 10 * n2;              /* 10*n^2 */
    if (n & 1)                          /* n odd => add 6 */
        expr += 6;
    /* prod = n * expr ; guaranteed divisible by 16 */
    seq_t prod = nseq * expr;
    return prod / 16;
}
