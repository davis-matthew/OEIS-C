/*
 * A229447  Number of 4 X n 0..2 arrays with horizontal differences mod 3
 *          never 1, vertical differences mod 3 never -1, and rows and columns
 *          lexicographically nondecreasing.
 * https://oeis.org/A229447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n^4 - 4*n^3 + 39*n^2 + 22*n + 84) / 12
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 202, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;            /* n as seq_t */
    seq_t n2 = nn * nn;             /* n^2 */
    seq_t n3 = n2 * nn;             /* n^3 */
    seq_t n4 = n2 * n2;             /* n^4 */

    seq_t num = 3 * n4 - 4 * n3 + 39 * n2 + 22 * nn + 84;
    return num / 12;
}
