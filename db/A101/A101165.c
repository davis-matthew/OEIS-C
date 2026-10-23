/*
 * A101165  a(n) = (7*n^3 + 6*n^2 + 5*n) / 6.
 * https://oeis.org/A101165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (7*n^3 + 6*n^2 + 5*n)/6
 * Range:     u8 n <= 2, u16 n <= 20, u32 n <= 864, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* convert index to seq_t */
    seq_t n2 = sn * sn;           /* n^2 */
    seq_t n3 = n2 * sn;           /* n^3 */
    seq_t num = 7 * n3 + 6 * n2 + 5 * sn; /* numerator */
    return num / 6;               /* exact division by 6 */
}
