/*
 * A134159  a(n) = 13 + 165*n + 756*n^2 + 1470*n^3 + 1029*n^4.
 * https://oeis.org/A134159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13 + 165*n + 756*n^2 + 1470*n^3 + 1029*n^4
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to the result type first to avoid lost‑precision */
    seq_t n1 = (seq_t)n;
    seq_t n2 = n1 * n1;          /* n^2   */
    seq_t n3 = n2 * n1;          /* n^3   */
    seq_t n4 = n3 * n1;          /* n^4   */

    seq_t res = 13;
    res += 165 * n1;
    res += 756 * n2;
    res += 1470 * n3;
    res += 1029 * n4;
    return res;
}
