/*
 * A222945  Number of distinct sums i+j+k with |i|, |j|, |k|, |i*j*k| <= n.
 * https://oeis.org/A222945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 7, a(n) = 4*n + 1 for n >= 2
 * Range:     u8 n <= 63, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;
    /* For all n >= 2, the count is 4*n + 1 */
    return (seq_t)4 * (seq_t)n + (seq_t)1;
}
