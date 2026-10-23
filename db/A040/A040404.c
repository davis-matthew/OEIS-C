/*
 * A040404  Continued fraction for sqrt(425).
 * https://oeis.org/A040404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0) = 20; for n ≥ 1, a(n) = 40 if 7 divides n, otherwise a(n) =
 *            1 (multiplicative: a(p^e)=1 for p≠7, a(7^e)=40)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)20;
    if (n % 7 == 0) return (seq_t)40;
    return (seq_t)1;
}
