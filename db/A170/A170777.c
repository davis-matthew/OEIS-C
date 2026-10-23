/*
 * A170777  a(n) = n^8*(n^5 + 1)/2.
 * https://oeis.org/A170777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^8 * (n^5 + 1) / 2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 31, u128 n <= 970,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;               /* cast idx_t to seq_t */
    seq_t n2 = nn * nn;         /* n^2 */
    seq_t n4 = n2 * n2;         /* n^4 */
    seq_t n5 = n4 * nn;         /* n^5 */
    seq_t n8 = n4 * n4;         /* n^8 */

    if ((nn & 1) == 0)          /* n even */
        return (n8 / 2) * (n5 + 1);
    else                        /* n odd */
        return ((n5 + 1) / 2) * n8;
}
