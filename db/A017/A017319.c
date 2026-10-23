/*
 * A017319  a(n) = (10*n + 4)^3.
 * https://oeis.org/A017319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 4)^3
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 162, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;      /* cast index to seq_t */
    x = x * (seq_t)10 + (seq_t)4; /* compute 10*n + 4 */
    seq_t y = x * x;        /* x^2 */
    return y * x;           /* x^3 */
}
