/*
 * A127111  a(n) = (n+1)! - (n)!!.
 * https://oeis.org/A127111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+1)! - n!!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (n+1)! */
    seq_t fact = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= n + (idx_t)1; ++i)
        fact = fact * (seq_t)i;

    /* Compute n!!, with convention 0!! = 1 */
    seq_t df = (seq_t)1;
    for (idx_t i = n; i >= (idx_t)2; i -= (idx_t)2)
        df = df * (seq_t)i;

    return fact - df;
}
