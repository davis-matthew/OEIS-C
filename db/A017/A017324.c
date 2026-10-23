/*
 * A017324  a(n) = (10*n + 4)^8.
 * https://oeis.org/A017324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 4)^8
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 25, u128 n <= 6553,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 10 + 4;
    seq_t base2 = base * base;
    seq_t base4 = base2 * base2;
    seq_t base8 = base4 * base4;
    return base8;
}
