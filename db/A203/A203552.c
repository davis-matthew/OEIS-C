/*
 * A203552  a(n) = n*(5*n^2 - 3*n + 4) / 6.
 * https://oeis.org/A203552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(5*n^2 - 3*n + 4)/6
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 1726, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;                /* n^2 */
    seq_t term = 5 * n2 - 3 * nn + 4;   /* 5*n^2 - 3*n + 4 */
    term *= nn;                         /* multiply by n */
    return term / 6;                    /* divide by 6 */
}
