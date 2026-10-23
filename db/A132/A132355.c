/*
 * A132355  Numbers of the form 9*h^2 + 2*h, for h an integer.
 * https://oeis.org/A132355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 - n + 5*floor(n/2)^2
 * Range:     u8 n <= 11, u16 n <= 171, u32 n <= 2108, u64 n <= 2108,
 *            u128 n <= 2108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t half = nn / 2;
    return nn * nn - nn + (seq_t)5 * half * half;
}
