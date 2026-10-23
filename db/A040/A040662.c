/*
 * A040662  Continued fraction for sqrt(689).
 * https://oeis.org/A040662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=26; for n≥1, a(n)=4 if n is odd, otherwise 52
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)26;
    return (n & 1) ? (seq_t)4 : (seq_t)52;
}
