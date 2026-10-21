/*
 * A229154  The clubs patterns appearing in n X n coins, with rotation
 *          allowed.
 * https://oeis.org/A229154
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^2/3), a(3) = 2
 * Range:     u8 n <= 27, u16 n <= 443, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 3) return (seq_t)2;
    seq_t nn = (seq_t)n;
    return nn * nn / 3;
}
