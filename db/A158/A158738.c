/*
 * A158738  a(n) = 72*n^2 - 1.
 * https://oeis.org/A158738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 72*n^2 - 1
 * Range:     u8 n <= 1, u16 n <= 30, u32 n <= 7723, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cn = (seq_t)n;
    seq_t val = cn * cn;          /* n^2 */
    val = val * (seq_t)72;        /* 72*n^2 */
    val = val - 1;                /* 72*n^2 - 1 */
    return val;
}
