/*
 * A190578  a(n) = n^7 + n.
 * https://oeis.org/A190578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 + n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 565,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;
    seq_t pow = n_t * n_t;   /* n^2 */
    pow *= n_t;             /* n^3 */
    pow *= n_t;             /* n^4 */
    pow *= n_t;             /* n^5 */
    pow *= n_t;             /* n^6 */
    pow *= n_t;             /* n^7 */
    pow += n_t;             /* n^7 + n */
    return pow;
}
