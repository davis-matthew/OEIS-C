/*
 * A007586  11-gonal (or hendecagonal) pyramidal numbers: a(n) =
 *          n*(n+1)*(3*n-2)/2.
 * https://oeis.org/A007586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*(n+1)*(3*n-2)/2
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t s_n = (seq_t)n;
    return s_n * (s_n + 1) / 2 * (3 * s_n - 2);
}
