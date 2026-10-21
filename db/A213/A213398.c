/*
 * A213398  Number of (w,x,y) with all terms in {0,...,n} and min(|w-x|,|x-y|)
 *          = x.
 * https://oeis.org/A213398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^2 + floor(n/2)
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t np1  = (seq_t)n + 1;      /* n+1 as seq_t                */
    seq_t sq   = np1 * np1;         /* (n+1)^2                     */
    seq_t half = (seq_t)(n / 2);    /* floor(n/2) – unsigned div  */
    return sq + half;              /* final result                */
}
