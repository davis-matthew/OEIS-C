/*
 * A130656  Interlacing n^3/2 and n^2(n + 1)/2.
 * https://oeis.org/A130656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2 * floor((n + 1)/2)
 * Range:     u8 n <= 7, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t f  = ((seq_t)n + 1) / 2;
    return nn * nn * f;
}
