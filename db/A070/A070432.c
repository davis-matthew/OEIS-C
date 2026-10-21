/*
 * A070432  Period 4: repeat [0, 1, 4, 1]; a(n) = n^2 mod 8.
 * https://oeis.org/A070432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n^2 mod 8
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t before multiplication to avoid overflow of idx_t */
    seq_t nn = (seq_t)n * (seq_t)n;
    return nn % 8;
}
