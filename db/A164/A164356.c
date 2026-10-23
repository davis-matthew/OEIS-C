/*
 * A164356  Expansion of (1 - x^2)^4 / ((1 - x)^4 * (1 - x^4)) in powers of x.
 * https://oeis.org/A164356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n≥1: a(n)=4 if n odd; a(n)=2 if n≡0 (mod4); a(n)=6
 *            if n≡2 (mod4)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)                  return (seq_t)1;
    if (n & 1)                   return (seq_t)4;   /* odd n */
    if ((n & 3) == 0)            return (seq_t)2;   /* n≡0 mod 4 */
    return (seq_t)6;             /* n≡2 mod 4 */
}
