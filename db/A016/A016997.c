/*
 * A016997  a(n) = (7*n + 1)^5.
 * https://oeis.org/A016997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 1)^5
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 11, u64 n <= 1018,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (7*n + 1)^5 using only integer arithmetic */
    seq_t t = ((seq_t)7 * (seq_t)n) + (seq_t)1;     /* base = 7*n + 1 */
    seq_t t2 = t * t;                               /* t^2 */
    seq_t t4 = t2 * t2;                             /* t^4 */
    return t4 * t;                                  /* t^5 */
}
