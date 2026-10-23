/*
 * A010197  Continued fraction for sqrt(138).
 * https://oeis.org/A010197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(n) = (n == 0) ? 11 : (n % 2 == 1) ? 1 : (n % 4 == 2) ? 2 : 22
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)11;
    if (n & 1)
        return (seq_t)1;        /* odd n */
    if ((n & 3) == 2)           /* n ≡ 2 mod 4 */
        return (seq_t)2;
    return (seq_t)22;           /* n ≡ 0 mod 4, n>0 */
}
