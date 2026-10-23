/*
 * A167191  a(n) = 4*n*(1 + 45*n + 620*n^2).
 * https://oeis.org/A167191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n*(1 + 45*n + 620*n^2)
 * Range:     u8 none, u16 n <= 2, u32 n <= 120, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n1 = (seq_t)n;
    seq_t n2 = n1 * n1;            /* n^2 */
    seq_t n3 = n2 * n1;            /* n^3 */
    seq_t res = 4 * n1 + 180 * n2 + 2480 * n3;
    return res;
}
