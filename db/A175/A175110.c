/*
 * A175110  a(n) = ((2*n+1)^4+1)/2.
 * https://oeis.org/A175110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((2*n+1)^4 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 142, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t to avoid accidental truncation. */
    seq_t t   = (seq_t)(2 * (seq_t)n + 1);   /* t = 2*n + 1          */
    seq_t t2  = t * t;                      /* t^2                  */
    seq_t t4  = t2 * t2;                    /* t^4                  */
    seq_t res = (t4 + 1) / 2;               /* ((2*n+1)^4 + 1)/2    */
    return res;
}
