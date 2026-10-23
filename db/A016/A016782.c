/*
 * A016782  a(n) = (3*n+1)^6.
 * https://oeis.org/A016782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n+1)^6
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 13, u64 n <= 541,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t first */
    seq_t xn = (seq_t)3 * (seq_t)n + (seq_t)1;   /* xn = 3*n + 1 */
    seq_t x2 = xn * xn;                         /* xn^2 */
    seq_t x3 = x2 * xn;                         /* xn^3 */
    seq_t x6 = x3 * x3;                         /* xn^6 */
    return x6;
}
