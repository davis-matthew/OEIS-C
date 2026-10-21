/*
 * A211704  a(n) = n + [n/2] + [n/3] + [n/4] + [n/5], where []=floor.
 * https://oeis.org/A211704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + [n/2] + [n/3] + [n/4] + [n/5]
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n + (seq_t)(n/2) + (seq_t)(n/3) + (seq_t)(n/4) + (seq_t)(n/5);
}
