/*
 * A229152  a(n) = n^9*(11*n+9)/2.
 * https://oeis.org/A229152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^9 * (11*n + 9) / 2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 71, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;          /* cast index to seq_t */
    seq_t power = 1;                /* power = n^9 */

    for (int i = 0; i < 9; ++i)
        power *= nseq;              /* multiply 9 times */

    seq_t factor = 11 * nseq + 9;   /* factor = 11*n + 9 */

    /* the product is always even; divide the even part by 2 first
       to avoid overflow in the multiplication */
    if ((power & 1) == 0)           /* power is even? */
        power >>= 1;
    else
        factor >>= 1;

    return power * factor;          /* final result */
}
