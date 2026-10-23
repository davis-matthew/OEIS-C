/*
 * A040168  Continued fraction for sqrt(182).
 * https://oeis.org/A040168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 13, a(n) = 26 if n is even and n ≥ 1, a(n) = 2 if n is
 *            odd and n ≥ 1
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)13;
    return (n % 2 == 0) ? (seq_t)26 : (seq_t)2;
}
