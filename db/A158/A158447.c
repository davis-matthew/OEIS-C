/*
 * A158447  a(n) = 10*n^2 - 1.
 * https://oeis.org/A158447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*n^2 - 1
 * Range:     u8 n <= 5, u16 n <= 80, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the result type, then compute 10*n^2-1 */
    seq_t v = (seq_t)n;
    seq_t t = v * v;     /* n^2 */
    t *= (seq_t)10;      /* 10*n^2 */
    t -= (seq_t)1;       /* 10*n^2 - 1 */
    return t;
}
