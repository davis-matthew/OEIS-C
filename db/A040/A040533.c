/*
 * A040533  Continued fraction for sqrt(557).
 * https://oeis.org/A040533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less, mult
 * Formula:   a(n) = 1 + 45*(⌊n/5⌋ - ⌊(n-1)/5⌋), a(0)=23
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)23;
    return (n % 5 == 0) ? (seq_t)46 : (seq_t)1;
}
