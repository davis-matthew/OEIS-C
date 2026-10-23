/*
 * A016855  a(n) = (5*n)^7.
 * https://oeis.org/A016855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n)^7
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 4, u64 n <= 113, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (5*n)^7 using repeated multiplication. */
    seq_t nn = (seq_t)n * (seq_t)5;   /* 5*n */
    seq_t r  = nn * nn;              /* (5*n)^2 */
    r *= nn;                         /* (5*n)^3 */
    r *= nn;                         /* (5*n)^4 */
    r *= nn;                         /* (5*n)^5 */
    r *= nn;                         /* (5*n)^6 */
    r *= nn;                         /* (5*n)^7 */
    return r;
}
