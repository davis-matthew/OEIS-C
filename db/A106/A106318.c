/*
 * A106318  Bhaskara twins: n such that 2*n^2 = X^3 and 2*n^3 = Y^2.
 * https://oeis.org/A106318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn, easy
 * Formula:   a(n) = 2*n^6
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t x2 = x * x;   /* n^2 */
    seq_t x4 = x2 * x2; /* n^4 */
    seq_t x6 = x4 * x2; /* n^6 */
    return 2 * x6;
}
