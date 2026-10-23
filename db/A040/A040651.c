/*
 * A040651  Continued fraction for sqrt(678).
 * https://oeis.org/A040651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=26; for n>=1, a(n)=26 if n is odd, 52 if n is even
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)26;
    return (seq_t)((n & 1ULL) ? 26 : 52);
}
