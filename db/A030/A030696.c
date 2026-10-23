/*
 * A030696  Cube root of A030695.
 * https://oeis.org/A030696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 10*n for n>=5; a(1)=5, a(2)=20, a(3)=14, a(4)=40
 * Range:     u8 n <= 25, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)20;
    if (n == 3) return (seq_t)14;
    if (n == 4) return (seq_t)40;
    return (seq_t)n * 10;
}
