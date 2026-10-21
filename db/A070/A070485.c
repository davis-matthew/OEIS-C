/*
 * A070485  a(n) = n^3 mod 23.
 * https://oeis.org/A070485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 23
 * Range:     u8 n <= 257, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m   = (seq_t)n % 23;        /* reduce n modulo 23 */
    seq_t res = m * m % 23;          /* (n^2) mod 23  */
    res = res * m % 23;              /* (n^3) mod 23  */
    return res;
}
