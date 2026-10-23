/*
 * A027602  a(n) = n^3 + (n+1)^3 + (n+2)^3.
 * https://oeis.org/A027602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 + (n+1)^3 + (n+2)^3
 * Range:     u8 n <= 3, u16 n <= 26, u32 n <= 750, u64 n <= 750,
 *            u128 n <= 750, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = n;                /* cast idx_t to seq_t */
    seq_t x2 = x * x;           /* n^2   */
    seq_t x3 = x2 * x;          /* n^3   */
    return 3 * x3 + 9 * x2 + 15 * x + 9;
}
