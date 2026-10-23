/*
 * A040684  Continued fraction for sqrt(711).
 * https://oeis.org/A040684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0) = 26; a(n) = 52 if 4 | n (and n > 0), otherwise a(n) = 1.
 *            This follows from the multiplicative definition: a(2) = 1,
 *            a(2^e) = 52 for e≥2, a(p^e) = 1 for each odd prime p
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)26;
    if ((n & 3u) == 0u)           /* n is divisible by 4 */
        return (seq_t)52;
    return (seq_t)1;
}
