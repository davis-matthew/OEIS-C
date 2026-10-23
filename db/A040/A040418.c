/*
 * A040418  Continued fraction for sqrt(439).
 * https://oeis.org/A040418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0) = 20, a(n) = 1 if n is odd, a(n) = 19 if n ≡ 2 (mod 4),
 *            a(n) = 40 if n ≡ 0 (mod 4) and n > 0
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)20;
    /* n is at least 1 now */
    if ((n & 1) != 0)          /* odd */
        return (seq_t)1;
    if ((n & 3) == 2)          /* n % 4 == 2 */
        return (seq_t)19;
    /* remaining case: n % 4 == 0 and n > 0 */
    return (seq_t)40;
}
