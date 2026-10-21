/*
 * A221465  Number of 0..n arrays of length 6 with each element unequal to at
 *          least one neighbor, starting with 0.
 * https://oeis.org/A221465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^5 + 3*n^4 + n^3
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 83, u64 n <= 136, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n4 = n3 * nn;          /* n^4 */
    seq_t n5 = n4 * nn;          /* n^5 */
    return n5 + (seq_t)3 * n4 + n3;
}
