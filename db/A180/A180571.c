/*
 * A180571  The Wiener index of the graph \|/_\/_\/_..._\/_\|/ having n nodes
 *          on the horizontal path.
 * https://oeis.org/A180571
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2 + 9*n + 18*n^2 + 3*n^3)/2
 * Range:     u8 n <= 2, u16 n <= 26, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn  = (seq_t)n;      /* cast index to arithmetic type */
    seq_t n2  = tn * tn;       /* n^2 */
    seq_t n3  = n2 * tn;       /* n^3 */

    /* numerator: 2 + 9*n + 18*n^2 + 3*n^3 */
    seq_t num = (seq_t)2
              + (seq_t)9 * tn
              + (seq_t)18 * n2
              + (seq_t)3 * n3;

    return num / 2;            /* integer division, exact */
}
