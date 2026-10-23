/*
 * A123919  Number of numbers congruent to 2 or 4 mod 6 and <= n.
 * https://oeis.org/A123919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(n/2) - floor(n/6)
 * Range:     u8 n <= 269, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (nn / 2) - (nn / 6);
}
