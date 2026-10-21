/*
 * A060494  a(n) = floor(n^4/64).
 * https://oeis.org/A060494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(n^4/64)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 330, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = nn * nn * nn * nn;
    return val / 64;
}
