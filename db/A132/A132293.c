/*
 * A132293  Maximal number of right angles in an n-gon.
 * https://oeis.org/A132293
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, more, uned, obsc
 * Formula:   a(n) = n-2 for n≥5; a(3)=1; a(4)=4
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 3) return (seq_t)1;
    if (n == 4) return (seq_t)4;
    return (seq_t)n - (seq_t)2;
}
