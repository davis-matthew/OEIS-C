/*
 * A219286  Number of n X 3 arrays of the minimum value of corresponding
 *          elements and their horizontal or vertical neighbors in a random,
 *          but sorted with lexicographically nondecreasing rows and columns,
 *          0..1 n X 3 array.
 * https://oeis.org/A219286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 6*n^3 - n^2 + 162*n - 264)/24 for n>2; a(1)=3,
 *            a(2)=6
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return (n == 1) ? (seq_t)3 : (seq_t)6;

    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;            /* n^2 */
    seq_t n3 = n2 * nn;            /* n^3 */
    seq_t n4 = n3 * nn;            /* n^4 */

    seq_t num = n4;
    num += (seq_t)6 * n3;
    num -= n2;
    num += (seq_t)162 * nn;
    num -= (seq_t)264;

    return num / (seq_t)24;
}
