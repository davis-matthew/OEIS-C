/*
 * A040367  Continued fraction for sqrt(387).
 * https://oeis.org/A040367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=19; multiplicative with a(2^e)=2, a(3^e)=19, a(p^e)=1 for
 *            p>=5
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)19;
    seq_t res = 1;
    if ((n & 1) == 0) res *= 2;          /* factor 2, any exponent gives 2 */
    if (n % 3 == 0) res *= 19;           /* factor 3, any exponent gives 19 */
    return res;
}
