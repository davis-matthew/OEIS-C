/*
 * A069474  First differences of A069473.
 * https://oeis.org/A069474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 120*n^3 + 540*n^2 + 900*n + 540
 * Range:     u8 none, u16 n <= 6, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = 120 * nn + 540;   /* 120*n + 540 */
    res = res * nn + 900;          /* (120*n + 540)*n + 900 */
    res = res * nn + 540;          /* ((120*n + 540)*n + 900)*n + 540 */
    return res;
}
