/*
 * A213846  Antidiagonal sums of the convolution array A213844.
 * https://oeis.org/A213846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^4 + 6*n^3 + 5*n^2 + 3*n)/6
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 228, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but guard against n==0 */
    seq_t nn = (seq_t)n;           /* cast index to seq_t type */
    seq_t n2 = nn * nn;            /* n^2 */
    seq_t n3 = n2 * nn;            /* n^3 */
    seq_t n4 = n3 * nn;            /* n^4 */
    seq_t num = 4 * n4 + 6 * n3 + 5 * n2 + 3 * nn; /* numerator of the fraction */
    return num / 6;                /* exact division by 6 (always divisible) */
}
