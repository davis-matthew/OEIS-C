/*
 * A198400  a(n) = 10^n - 10*n.
 * https://oeis.org/A198400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n - 10*n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow = 1;
    for (idx_t i = 0; i < n; ++i)
        pow *= (seq_t)10;                /* compute 10^n */
    seq_t sub = (seq_t)10 * (seq_t)n;    /* compute 10*n */
    return pow - sub;                   /* a(n) = 10^n - 10*n */
}
