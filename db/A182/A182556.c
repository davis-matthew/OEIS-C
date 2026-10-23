/*
 * A182556  a(0)=1, a(n+1) = (a(n)*6) XOR a(n).
 * https://oeis.org/A182556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0)=1; a(n+1) = (a(n)*6) XOR a(n)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 1;
    for (idx_t i = 0; i < n; ++i)
        val = (val * (seq_t)6) ^ val;
    return val;
}
