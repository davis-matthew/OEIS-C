/*
 * A017201  a(n) = (9*n + 3)^5.
 * https://oeis.org/A017201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 3)^5
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 9, u64 n <= 792, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n * 9ULL + 3ULL;      /* compute (9*n + 3) */
    seq_t t = x * x;                      /* x^2 */
    t = t * t;                            /* x^4 */
    t = t * x;                            /* x^5 */
    return t;
}
