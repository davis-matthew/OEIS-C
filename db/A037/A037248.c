/*
 * A037248  a(n) = n^6*(n^2 - 1)*(n^6 - 1).
 * https://oeis.org/A037248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6*(n^2 - 1)*(n^6 - 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 23, u128 n <= 565,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n; /* convert index to seq_t */
    if (sn == 0 || sn == 1)
    {
        return 0;
    }
    seq_t n2 = sn * sn;        /* n^2 */
    seq_t n3 = n2 * sn;        /* n^3 */
    seq_t n6 = n3 * n3;        /* n^6 */
    seq_t part2 = n2 - 1;      /* n^2 - 1 */
    seq_t part3 = n6 - 1;      /* n^6 - 1 */
    return n6 * part2 * part3; /* product */
}
