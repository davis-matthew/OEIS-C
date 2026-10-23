/*
 * A017573  a(n) = (12n+4)^5.
 * https://oeis.org/A017573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12n+4)^5
 * Range:     u8 none, u16 n <= 0, u32 n <= 6, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute (12*n + 4)^5 using only seq_t arithmetic */
    seq_t x = (seq_t)n;          /* n */
    x = x * (seq_t)12 + (seq_t)4;/* 12*n + 4 */

    /* compute powers by repeated squaring to reduce multiplications */
    seq_t x2 = x * x;            /* (12n+4)^2  */
    seq_t x4 = x2 * x2;          /* (12n+4)^4  */
    return x4 * x;               /* (12n+4)^5  */
}
