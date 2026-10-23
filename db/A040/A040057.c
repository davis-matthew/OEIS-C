/*
 * A040057  Continued fraction for sqrt(66).
 * https://oeis.org/A040057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=8; for n≥1: if n is odd a(n)=8 else a(n)=16
 * Range:     u8 n <= 9733, u16 n <= 9733, u32 n <= 9733, u64 n <= 9733,
 *            u128 n <= 9733, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)8;
    return (n & 1) ? (seq_t)8 : (seq_t)16;
}
