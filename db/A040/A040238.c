/*
 * A040238  Continued fraction for sqrt(254).
 * https://oeis.org/A040238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0) = 15; for n > 0, a(n) = 1 if n is odd, a(n) = 14 if n ≡ 2
 *            (mod 4), and a(n) = 30 if n ≡ 0 (mod 4)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)15;
    if ((n & (idx_t)1) == (idx_t)0) {        /* n even */
        if ((n & (idx_t)2) == (idx_t)0)      /* divisible by 4 */
            return (seq_t)30;
        else                                 /* n ≡ 2 (mod 4) */
            return (seq_t)14;
    }
    return (seq_t)1;                          /* n odd */
}
