/*
 * A040783  Continued fraction for sqrt(812).
 * https://oeis.org/A040783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=28, a(n)= (n%2?2:56) for n>0
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)28;
    return (n & 1) ? (seq_t)2 : (seq_t)56;
}
