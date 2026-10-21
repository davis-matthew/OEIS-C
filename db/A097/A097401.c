/*
 * A097401  Largest achievable determinant of a 3 X 3 matrix whose elements
 *          are 9 distinct nonnegative integers chosen from the range 0..n.
 * https://oeis.org/A097401
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^3 - 18*n^2 + 68*n - 84
 * Range:     u8 none, u16 n <= 34, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n1 = (seq_t)n;
    seq_t n2 = n1 * n1;          /* n^2   */
    seq_t n3 = n2 * n1;          /* n^3   */

    /* compute 2*n^3 + 68*n first to avoid unsigned underflow */
    seq_t cur = 2 * n3 + 68 * n1;
    /* subtract 18*n^2 + 84 */
    cur -= (18 * n2 + (seq_t)84);

    return cur;
}
