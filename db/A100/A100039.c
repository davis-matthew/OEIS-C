/*
 * A100039  Positions of occurrences of the natural numbers as fourth
 *          subsequence in A100035.
 * https://oeis.org/A100039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n^2 + 7*n + 13
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1, but values for n < 1 are irrelevant */
    seq_t N = (seq_t)n;
    return (seq_t)2 * N * N + (seq_t)7 * N + (seq_t)13;
}
