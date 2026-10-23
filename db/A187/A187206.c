/*
 * A187206  a(n) = 6*(24*n - 1).
 * https://oeis.org/A187206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*(24*n - 1)
 * Range:     u8 n <= 1, u16 n <= 455, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* behaviour for n < 1 is undefined; return 0 */
        return 0;
    }
    seq_t temp = (seq_t)n * 24 - 1;
    return temp * 6;
}
