/*
 * A229440  Number of n X 3 0..2 arrays with horizontal differences mod 3
 *          never 1, vertical differences mod 3 never -1, and rows and columns
 *          lexicographically nondecreasing.
 * https://oeis.org/A229440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^6 + 3*n^5 + 10*n^4 + 75*n^3 - 11*n^2 + 1002*n + 720) /
 *            360
 * Range:     u8 none, u16 n <= 5, u32 n <= 49, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t n2 = N * N;         /* n^2 */
    seq_t n3 = n2 * N;        /* n^3 */
    seq_t n4 = n3 * N;        /* n^4 */
    seq_t n5 = n4 * N;        /* n^5 */
    seq_t n6 = n5 * N;        /* n^6 */

    /* numerator = n^6 + 3*n^5 + 10*n^4 + 75*n^3 - 11*n^2 + 1002*n + 720 */
    seq_t num = n6;
    num += 3 * n5;
    num += 10 * n4;
    num += 75 * n3;
    num -= 11 * n2;          /* subtract 11*n^2 */
    num += 1002 * N;
    num += 720;              /* constant term */

    const seq_t denom = 360;
    return num / denom;
}
