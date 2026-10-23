/*
 * A040151  Continued fraction for sqrt(164).
 * https://oeis.org/A040151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(n) = 12 for n = 0; otherwise a(n) = 4^(2|n) * 6^(3|n), i.e.
 *            return 1 if n has no 2 or 3 in its prime factorization, return 4
 *            if only 2 divides n, 6 if only 3 divides n, and 24 if both 2 and
 *            3 divide n
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)12;
    seq_t val = 1;
    if ((n & 1u) == 0) val *= (seq_t)4;   /* 2 divides n */
    if (n % 3 == 0)    val *= (seq_t)6;   /* 3 divides n */
    return val;
}
