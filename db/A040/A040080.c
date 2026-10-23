/*
 * A040080  Continued fraction for sqrt(90).
 * https://oeis.org/A040080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   Continued fraction for sqrt(90): a(0) = 9 For n > 0: a(n) = 2 if
 *            n is odd, otherwise 18
 * Range:     u8 n <= 9430, u16 n <= 9430, u32 n <= 9430, u64 n <= 9430,
 *            u128 n <= 9430, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 9;
    return (n & 1) ? 2 : 18;
}
