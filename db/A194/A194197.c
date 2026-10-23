/*
 * A194197  Number of partitions of 60n into parts <= 6.
 * https://oeis.org/A194197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + (167*n +2325*n^2 +15400*n^3 +47250*n^4 +54000*n^5)/6
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 9, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n1 = n;
    seq_t n2 = n1 * n1;   /* n^2 */
    seq_t n3 = n2 * n1;   /* n^3 */
    seq_t n4 = n3 * n1;   /* n^4 */
    seq_t n5 = n4 * n1;   /* n^5 */

    seq_t sum = (seq_t)167 * n1
              + (seq_t)2325 * n2
              + (seq_t)15400 * n3
              + (seq_t)47250 * n4
              + (seq_t)54000 * n5;

    return 1 + sum / 6;
}
