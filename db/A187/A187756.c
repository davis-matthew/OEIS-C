/*
 * A187756  a(n) = n^2 * (4*n^2 - 1) / 3.
 * https://oeis.org/A187756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * (4*n^2 - 1) / 3
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 237, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t and compute using the explicit formula. */
    seq_t nn = (seq_t)n * (seq_t)n;                     /* n^2 */
    seq_t num = nn * (( (seq_t)4 * nn ) - 1);           /* n^2 * (4*n^2 - 1) */
    return num / 3;                                     /* divide by 3 */
}
