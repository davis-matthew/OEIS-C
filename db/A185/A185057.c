/*
 * A185057  a(n) = n^n! (mod 5).
 * https://oeis.org/A185057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^n! (mod 5)
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first four values explicitly */
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;
    if (n == 2 || n == 3)
        return (seq_t)4;

    /* For n >= 4 the exponent n! is divisible by 4,
       so a(n) depends only on n modulo 5. */
    if (n % 5 == 0)
        return (seq_t)0;
    return (seq_t)1;
}
