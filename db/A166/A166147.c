/*
 * A166147  a(n) = 4*n^2 + 4*n - 7.
 * https://oeis.org/A166147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^2 + 4*n - 7
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset: for n = 0 the value is undefined; return 0 */
    if (n == 0)
        return (seq_t)0;

    seq_t nn = (seq_t)n;
    /* Compute 4*n^2 + 4*n - 7 */
    seq_t val = 4 * nn * nn + 4 * nn - (seq_t)7;
    return val;
}
