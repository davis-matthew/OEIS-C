/*
 * A106614  a(n) = numerator of n/(n+13).
 * https://oeis.org/A106614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, mult, easy
 * Formula:   a(n) = n if 13∤n, a(13·k) = k
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if ((n % 13) == 0)
        return nn / 13;
    return nn;
}
