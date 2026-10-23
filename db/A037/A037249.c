/*
 * A037249  a(n) = n^24*(n^2-1)*(n^6-1)*(n^8-1)*(n^12-1).
 * https://oeis.org/A037249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^24*(n^2-1)*(n^6-1)*(n^8-1)*(n^12-1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;          /* n^2 */
    seq_t n4 = n2 * n2;          /* n^4 */
    seq_t n6 = n4 * n2;          /* n^6 */
    seq_t n8 = n4 * n4;          /* n^8 */
    seq_t n12 = n6 * n6;         /* n^12 */
    seq_t n24 = n12 * n12;       /* n^24 */

    seq_t res = n24;
    res *= (n2 - 1);
    res *= (n6 - 1);
    res *= (n8 - 1);
    res *= (n12 - 1);
    return res;
}
