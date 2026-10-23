/*
 * A140227  Binomial transform of [1, 4, 6, 4, 1, 1, -1, 1, -1, 1, ...].
 * https://oeis.org/A140227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = 2 + 35*(n-1)^2/12 + (n-1)^4/12
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    seq_t k = (seq_t)n - (seq_t)1;            /* k = n-1 */
    seq_t k2 = k * k;                         /* k^2 */
    seq_t k4 = k2 * k2;                       /* k^4 */
    seq_t num = (seq_t)35 * k2 + k4;          /* 35*k^2 + k^4 */
    /* division by 12 is exact for all integers k */
    seq_t res = (seq_t)2 + num / (seq_t)12;
    return res;
}
