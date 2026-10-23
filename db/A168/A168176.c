/*
 * A168176  a(n) = n^2*(n^10 + 1)/2.
 * https://oeis.org/A168176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 * (n^10 + 1) / 2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 42, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed exactly when the result fits in seq_t.  */
seq_t a(idx_t n)
{
    seq_t nn      = (seq_t)n;          /* convert index to seq_t */
    seq_t n_sq    = nn * nn;           /* n^2 */

    /* compute n^5 */
    seq_t n_pow5  = nn;
    for (idx_t i = 1; i < 5; ++i)
        n_pow5 = n_pow5 * nn;          /* n^5 */

    seq_t n_pow10 = n_pow5 * n_pow5;    /* n^10 */

    /* Divide by 2 before the final multiplication to avoid overflow.
       For odd n, (n^10 + 1) is even; for even n, n^2 is even. */
    if (n & 1) {                       /* n is odd */
        seq_t temp = (n_pow10 + 1) >> 1;   /* (n^10 + 1)/2  */
        return temp * n_sq;                /* * n^2          */
    } else {                           /* n is even     */
        seq_t temp = n_sq >> 1;             /* n^2 / 2        */
        return temp * (n_pow10 + 1);        /* * (n^10 + 1)   */
    }
}
