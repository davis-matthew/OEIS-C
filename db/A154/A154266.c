/*
 * A154266  a(n) = 27*n + 12.
 * https://oeis.org/A154266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27*n + 12
 * Range:     u8 n <= 9, u16 n <= 2426, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 27 * nn + 12;
}
