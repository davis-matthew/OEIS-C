/*
 * A017310  a(n) = (10*n + 3)^6.
 * https://oeis.org/A017310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 3)^6
 * Range:     u8 none, u16 n <= 0, u32 n <= 3, u64 n <= 162, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)10 * n + 3;   /* base = 10*n + 3 */
    seq_t sq = t * t;              /* t^2 */
    seq_t sq2 = sq * sq;           /* t^4 */
    seq_t res = sq2 * sq;          /* t^6 = t^4 * t^2 */
    return res;
}
