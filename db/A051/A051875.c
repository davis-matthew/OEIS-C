/*
 * A051875  23-gonal numbers: a(n) = n(21n-19)/2.
 * https://oeis.org/A051875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(21*n-19)/2
 * Range:     u8 n <= 5, u16 n <= 79, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t before performing calculations.
       All operations stay within seq_t, so the result is exact
       as long as it fits in the seq_t type. */
    seq_t n_t  = (seq_t)n;
    seq_t tmp  = (seq_t)21 * n_t - (seq_t)19;   /* 21*n - 19 */
    seq_t res  = n_t * tmp / (seq_t)2;          /* n*(21*n-19)/2 */
    return res;
}
