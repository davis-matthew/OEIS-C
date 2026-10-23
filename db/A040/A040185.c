/*
 * A040185  Continued fraction for sqrt(200).
 * https://oeis.org/A040185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(0) = 14
    for n > 0: a(n) = 7 if n is odd, 28 if n is even (period 2  of continued fraction of sqrt(200)) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)14;
    return (n & 1) ? (seq_t)7 : (seq_t)28;
}
