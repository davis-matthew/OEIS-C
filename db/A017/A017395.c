/*
 * A017395  a(n) = (11*n)^7.
 * https://oeis.org/A017395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n)^7
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 51, u128 n <= 500,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n * 11;      /* (11*n) */
    seq_t t2 = t * t;             /* (11*n)^2 */
    seq_t t3 = t2 * t;            /* (11*n)^3 */
    seq_t t4 = t3 * t;            /* (11*n)^4 */
    seq_t t5 = t4 * t;            /* (11*n)^5 */
    seq_t t6 = t5 * t;            /* (11*n)^6 */
    seq_t t7 = t6 * t;            /* (11*n)^7 */
    return t7;
}
