/*
 * A100176  Structured octagonal prism numbers.
 * https://oeis.org/A100176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^3 - 2*n^2
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 1127, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;        /* n^2 */
    seq_t n3 = n2 * nn;        /* n^3 */
    return 3 * n3 - 2 * n2;
}
