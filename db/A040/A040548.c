/*
 * A040548  Continued fraction for sqrt(572).
 * https://oeis.org/A040548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less, mult
 * Formula:   a(0) = 23; for n ≥ 1: a(n) = 1 if n is odd, a(n) = 10 if n ≡ 2
 *            (mod 4), and a(n) = 46 if n ≡ 0 (mod 4)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)23;
    if ((n & (idx_t)1) != 0) return (seq_t)1;      /* odd n → 1 */
    if ((n & (idx_t)3) == (idx_t)2) return (seq_t)10; /* n≡2 mod 4 →10 */
    return (seq_t)46;                               /* n≡0 mod 4 →46 */
}
