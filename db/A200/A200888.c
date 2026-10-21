/*
 * A200888  Number of 0..n arrays x(0..4) of 5 elements without any interior
 *          element greater than both neighbors.
 * https://oeis.org/A200888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n^5 + 55*n^4 + 175*n^3 + 230*n^2 + 136*n + 30) / 30
 *            (valid for n ≥ 1, offset 1)
 * Range:     u8 none, u16 n <= 4, u32 n <= 97, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* below the offset; behaviour undefined per OEIS */

    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n4 = n3 * nn;          /* n^4 */
    seq_t n5 = n4 * nn;          /* n^5 */

    seq_t num = 4 * n5 + 55 * n4 + 175 * n3 + 230 * n2 + 136 * nn + 30;
    return num / 30;             /* exact division */
}
