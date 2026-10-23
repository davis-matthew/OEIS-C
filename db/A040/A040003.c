/*
 * A040003  Continued fraction for sqrt(6).
 * https://oeis.org/A040003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=2, a(1)=2, a(2k)=4 for k≥1, a(2k+1)=2 for k≥0
 * Range:     u8 n <= 9282, u16 n <= 9282, u32 n <= 9282, u64 n <= 9282,
 *            u128 n <= 9282, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 2;
    return (n & 1ULL) ? 2 : 4;
}
