/*
 * A010014  a(0) = 1, a(n) = 24*n^2 + 2 for n>0.
 * https://oeis.org/A010014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 24*n^2 + 2 for n > 0
 * Range:     u8 n <= 3, u16 n <= 52, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t nn = (seq_t)n;          /* cast first to avoid mixing types */
    return 24 * nn * nn + 2;    /* 24*n^2 + 2 */
}
