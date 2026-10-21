/*
 * A069074  a(n) = (2*n+2)*(2*n+3)*(2*n+4) = 24*A000330(n+1).
 * https://oeis.org/A069074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n+2)*(2*n+3)*(2*n+4)
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 811, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t first */
    seq_t x = (seq_t)n;

    /* Compute the three consecutive terms */
    seq_t term1 = (x + (seq_t)1) * (seq_t)2;    /* 2*n + 2 */
    seq_t term2 = (x * (seq_t)2) + (seq_t)3;    /* 2*n + 3 */
    seq_t term3 = (x * (seq_t)2) + (seq_t)4;    /* 2*n + 4 */

    /* Multiply them together using seq_t arithmetic */
    return term1 * term2 * term3;
}
