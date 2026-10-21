/*
 * A059839  a(n) = n^8 + n^6 + n^4 + n^2 + 1.
 * https://oeis.org/A059839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^8 + n^6 + n^4 + n^2 + 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 255, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert index to the sequence value type */
    seq_t sn = (seq_t)n;

    /* compute successive powers of n */
    seq_t n2 = sn * sn;        /* n^2 */
    seq_t n4 = n2 * n2;        /* n^4 */
    seq_t n6 = n4 * n2;        /* n^6 */
    seq_t n8 = n4 * n4;        /* n^8 */

    return n8 + n6 + n4 + n2 + (seq_t)1;
}
