/*
 * A010194  Continued fraction for sqrt(135).
 * https://oeis.org/A010194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=11; a(n) = 22 if n % 8 == 0 and n > 0, otherwise 1
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)11;
    if ((n & (idx_t)7) == (idx_t)0)
        return (seq_t)22;
    return (seq_t)1;
}
