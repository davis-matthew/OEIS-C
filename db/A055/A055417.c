/*
 * A055417  Number of points in N^n of norm <= 2.
 * https://oeis.org/A055417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3 - 3*n^2 + 14*n + 24)*(n+1)/24
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 276, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t first */
    seq_t n1 = (seq_t)n;
    seq_t n2 = n1 * n1;          /* n^2 */
    seq_t n3 = n2 * n1;          /* n^3 */

    /* Compute the numerator polynomial: n^3 - 3*n^2 + 14*n + 24 */
    seq_t tmp = n3 - (seq_t)3 * n2 + (seq_t)14 * n1 + (seq_t)24;

    /* Multiply by (n+1) and divide by 24 */
    seq_t prod = tmp * (n1 + (seq_t)1);
    return prod / (seq_t)24;
}
