/*
 * A146308  a(n) is the smallest k such that the numerator of (k-6)/(2k)
 *          equals n.
 * https://oeis.org/A146308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = 12*n+6 if n≡0 (mod 6); * a(n) = 3*n+6 if n≡0 (mod 3) but
 *            not 6; * a(n) = 4*n+6 if n≡0 (mod 2) but not 3; * a(n) = n+6
 *            otherwise
 * Range:     u8 n <= 23, u16 n <= 5465, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n % 6 == 0)
        return (seq_t)12 * (seq_t)n + (seq_t)6;
    else if (n % 3 == 0)
        return (seq_t)3 * (seq_t)n + (seq_t)6;
    else if (n % 2 == 0)
        return (seq_t)4 * (seq_t)n + (seq_t)6;
    else
        return (seq_t)n + (seq_t)6;
}
