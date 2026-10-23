/*
 * A025767  Expansion of 1/((1-x)*(1-x^3)*(1-x^4)).
 * https://oeis.org/A025767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + 8n + 24)/24)
 * Range:     u8 n <= 11, u16 n <= 252, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t num = nn * nn + 8 * nn + 24;
    return num / 24;
}
