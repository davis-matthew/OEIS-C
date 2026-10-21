/*
 * A223764  Number of n X 2 0..1 arrays with rows, columns and antidiagonals
 *          unimodal and diagonals nondecreasing.
 * https://oeis.org/A223764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^4/24 + n^3/4 + (35*n^2)/24 + (5*n)/4 + 1
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use the explicit polynomial formula derived for A223764. */
    seq_t x = (seq_t)n;          /* n  */
    seq_t x2 = x * x;            /* n^2 */
    seq_t x3 = x2 * x;           /* n^3 */
    seq_t x4 = x3 * x;           /* n^4 */

    /* Numerator: n^4 + 6*n^3 + 35*n^2 + 30*n */
    seq_t num = x4 + 6 * x3 + 35 * x2 + 30 * x;

    /* Final value: numerator / 24 + 1 */
    return (num / (seq_t)24) + (seq_t)1;
}
