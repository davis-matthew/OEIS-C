/*
 * A158548  a(n) = 169*n^2 + 13.
 * https://oeis.org/A158548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 169*n^2 + 13
 * Range:     u8 n <= 1, u16 n <= 19, u32 n <= 5041, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x  = (seq_t)n;      /* cast index to seq_t */
    seq_t res = (seq_t)169;   /* start with the coefficient 169 */

    res *= x;                 /* res = 169 * n          */
    res *= x;                 /* res = 169 * n * n      */
    res += (seq_t)13;         /* res = 169 * n^2 + 13   */

    return res;
}
