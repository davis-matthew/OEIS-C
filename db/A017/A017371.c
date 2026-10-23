/*
 * A017371  a(n) = (10*n + 8)^7.
 * https://oeis.org/A017371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 8)^7
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 55, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute base B = 10*n + 8 */
    seq_t B = (seq_t)n * (seq_t)10 + 8;
    /* compute B^7 by repeated multiplication */
    seq_t r = B;
    r *= B; /* B^2 */
    r *= B; /* B^3 */
    r *= B; /* B^4 */
    r *= B; /* B^5 */
    r *= B; /* B^6 */
    r *= B; /* B^7 */
    return r;
}
