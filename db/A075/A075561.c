/*
 * A075561  Domination number for kings' graph K(n).
 * https://oeis.org/A075561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+2)/3)^2
 * Range:     u8 n <= 45, u16 n <= 765, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t tmp = n + 2;   /* use idx_t for the arithmetic to avoid overflow */
    tmp /= 3;
    seq_t m = (seq_t)tmp;
    return m * m;
}
