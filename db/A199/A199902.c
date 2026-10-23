/*
 * A199902  Number of -n..n arrays x(0..6) of 7 elements with zero sum, and
 *          adjacent elements not both strictly positive and not both strictly
 *          negative.
 * https://oeis.org/A199902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (151/180)*n^6 + (163/15)*n^5 + (377/9)*n^4 + (395/6)*n^3
 *            + (7429/180)*n^2 + (93/10)*n + 1
 * Range:     u8 none, u16 n <= 1, u32 n <= 15, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;           /* cast index to the sequence type */
    seq_t n2 = sn * sn;            /* n^2   */
    seq_t n3 = n2 * sn;            /* n^3   */
    seq_t n4 = n3 * sn;            /* n^4   */
    seq_t n5 = n4 * sn;            /* n^5   */
    seq_t n6 = n5 * sn;            /* n^6   */
    /* build the numerator of the rational expression */
    seq_t num = 151 * n6 + 1956 * n5 + 7540 * n4 + 11850 * n3 + 7429 * n2
                + 1674 * sn + 180;
    return num / 180;              /* exact division – numerator is always a multiple of 180 */
}
