/*
 * A219514  Number of n X 3 arrays of the minimum value of corresponding
 *          elements and their horizontal, diagonal or antidiagonal neighbors
 *          in a random, but sorted with lexicographically nondecreasing rows
 *          and columns, 0..1 n X 3 array.
 * https://oeis.org/A219514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A219514
 * Formula: a(n) = (n^4 + 2*n^3 - 37*n^2 + 322*n - 552) / 24   for n > 3
 * Initial values: a(1)=3, a(2)=4, a(3)=11
 */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)11;

    seq_t nn  = (seq_t)n;          /* convert index to seq_t */
    seq_t n2  = nn * nn;           /* n^2 */
    seq_t n3  = n2 * nn;           /* n^3 */
    seq_t n4  = n3 * nn;           /* n^4 */

    /* numerator = n^4 + 2*n^3 + 322*n - (37*n^2 + 552) */
    seq_t pos = n4 + (seq_t)2 * n3 + (seq_t)322 * nn;
    seq_t neg = (seq_t)37 * n2 + (seq_t)552;
    seq_t num = pos - neg;         /* pos > neg for n > 3 */

    return num / (seq_t)24;
}
