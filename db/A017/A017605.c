/*
 * A017605  a(n) = 12*n + 7.
 * https://oeis.org/A017605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*n + 7
 * Range:     u8 n <= 20, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;
    val = val * 12 + 7;
    return val;
}
