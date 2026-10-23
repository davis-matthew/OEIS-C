/*
 * A158130  a(n) = 121*n - 1.
 * https://oeis.org/A158130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 121*n - 1
 * Range:     u8 n <= 2, u16 n <= 541, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* for n = 0 the sequence is undefined; return 0 */
    if (n == 0)
        return 0;
    return (seq_t)n * (seq_t)121 - 1;
}
