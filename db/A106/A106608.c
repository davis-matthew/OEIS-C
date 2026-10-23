/*
 * A106608  a(n) = numerator of n/(n+7).
 * https://oeis.org/A106608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac, mult
 * Formula:   a(n) = n / gcd(n,7) (i.e. a(n) = n if n mod 7 ≠ 0, otherwise n /
 *            7)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;          /* value before reduction */
    if ((n % (idx_t)7) == 0) {
        val /= (seq_t)7;           /* divide by 7 when 7 | n */
    }
    return val;
}
