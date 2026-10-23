/*
 * A016838  a(n) = (4*n + 3)^2.
 * https://oeis.org/A016838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n + 3)^2
 * Range:     u8 n <= 3, u16 n <= 63, u32 n <= 860, u64 n <= 860,
 *            u128 n <= 860, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n * 4 + 3;
    return t * t;
}
