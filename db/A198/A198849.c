/*
 * A198849  a(n) = (11*6^n - 1)/5.
 * https://oeis.org/A198849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*6^n - 1)/5; recurrence: a(0)=2; a(n)=6*a(n-1)+1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 2;                 /* a(0) */
    for (idx_t i = 0; i < n; ++i)  /* apply recurrence n times */
    {
        res = res * 6 + 1;
    }
    return res;
}
