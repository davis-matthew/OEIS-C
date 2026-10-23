/*
 * A187195  Number of 8-turn rook's tours on an n X n board summed over all
 *          starting positions.
 * https://oeis.org/A187195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 128*n^9 - 1538*n^8 + 7956*n^7 - 23556*n^6 + 44586*n^5 -
 *            55218*n^4 + 40894*n^3 - 13252*n^2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 8, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast index to seq_t */
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n3 = n2 * nn;           /* n^3 */
    seq_t n4 = n3 * nn;           /* n^4 */
    seq_t n5 = n4 * nn;           /* n^5 */
    seq_t n6 = n5 * nn;           /* n^6 */
    seq_t n7 = n6 * nn;           /* n^7 */
    seq_t n8 = n7 * nn;           /* n^8 */
    seq_t n9 = n8 * nn;           /* n^9 */

    return (seq_t)128 * n9
         - (seq_t)1538 * n8
         + (seq_t)7956 * n7
         - (seq_t)23556 * n6
         + (seq_t)44586 * n5
         - (seq_t)55218 * n4
         + (seq_t)40894 * n3
         - (seq_t)13252 * n2;
}
