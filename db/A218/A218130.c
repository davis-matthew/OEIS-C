/*
 * A218130  Number of length 6 primitive (=aperiodic or period 6) n-ary words.
 * https://oeis.org/A218130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 - n^3 - n^2 + n
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    seq_t n2 = m * m;        /* n^2 */
    seq_t n3 = n2 * m;       /* n^3 */
    seq_t n6 = n3 * n3;      /* n^6 */
    seq_t res = n6 - n3 - n2 + m;
    return res;
}
