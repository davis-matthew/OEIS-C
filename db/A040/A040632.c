/*
 * A040632  Continued fraction for sqrt(658).
 * https://oeis.org/A040632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult, less
 * Formula:   a(0)=25; for n>0, a(n)=50 if n≡0 (mod 8), 6 if n≡4 (mod 8), 1
 *            otherwise
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)25;
    idx_t r = n % ((idx_t)8);
    if (r == 0) return (seq_t)50;
    if (r == 4) return (seq_t)6;
    return (seq_t)1;
}
