/*
 * A011245  a(n) = (n+1)*(2*n+1)*(3*n+1)*(4*n+1).
 * https://oeis.org/A011245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(2*n+1)*(3*n+1)*(4*n+1)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 115, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    seq_t res = m + 1;
    res *= (2 * m + 1);
    res *= (3 * m + 1);
    res *= (4 * m + 1);
    return res;
}
