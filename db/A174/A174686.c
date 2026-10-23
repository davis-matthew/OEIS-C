/*
 * A174686  Number of equivalence classes of 3 X 3 matrices filled with n
 *          colors so that no two rotations are identical.
 * https://oeis.org/A174686
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^9 - n^5) / 4
 * Range:     u8 none, u16 n <= 3, u32 n <= 11, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* indices below the offset are undefined */
    seq_t sn  = (seq_t)n;          /* cast index to sequence type */
    seq_t n2  = sn * sn;          /* n^2 */
    seq_t n3  = n2 * sn;          /* n^3 */
    seq_t n4  = n2 * n2;          /* n^4 */
    seq_t n5  = n3 * n2;          /* n^5 = n^3 * n^2 */
    seq_t n9  = n5 * n4;          /* n^9 = n^5 * n^4 */
    seq_t diff = n9 - n5;         /* n^9 - n^5 */
    return diff / (seq_t)4;       /* divide by 4 */
}
