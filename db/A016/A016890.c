/*
 * A016890  a(n) = (5n+3)^6.
 * https://oeis.org/A016890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5n+3)^6
 * Range:     u8 none, u16 n <= 0, u32 n <= 7, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;            /* x = n                 */
    x = 5 * x + 3;                 /* x = 5n + 3            */
    seq_t y = x * x;               /* y = x^2               */
    seq_t z = y * y;               /* z = x^4               */
    return z * y;                  /* return x^6            */
}
