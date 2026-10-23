/*
 * A017562  a(n) = (12*n + 3)^6.
 * https://oeis.org/A017562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 3)^6
 * Range:     u8 none, u16 n <= 0, u32 n <= 3, u64 n <= 135, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute t = 12*n + 3 */
    seq_t t = (seq_t)n * 12 + 3;

    /* Compute t^2, t^3, then t^6 */
    seq_t t2 = t * t;       /* t^2 */
    seq_t t3 = t2 * t;      /* t^3 */
    seq_t res = t3 * t3;    /* (t^3)^2 = t^6 */

    return res;
}
