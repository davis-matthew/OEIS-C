/*
 * A078411  Expansion of Molien series for a certain 4-D group of order 48.
 * https://oeis.org/A078411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((2*n^3 + 6*n^2 + 27*n + 36) - 9*n*(n%2)) / 36
 * Range:     u8 n <= 3, u16 n <= 30, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A078411 */
seq_t a(idx_t n)
{
    seq_t nsq = (seq_t)n * (seq_t)n;             /* n^2 */
    seq_t ncub = nsq * (seq_t)n;                 /* n^3 */
    seq_t num = 2 * ncub + 6 * nsq + 27 * (seq_t)n + 36;
    seq_t sub = ((n & 1) ? 9 * (seq_t)n : 0);
    return (num - sub) / 36;
}
