/*
 * A040399  Continued fraction for sqrt(420).
 * https://oeis.org/A040399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=20, a(n)=2 if n odd, 40 if n even (for n>=1)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)20;
    return (n & (idx_t)1) ? (seq_t)2 : (seq_t)40;
}
