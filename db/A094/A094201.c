/*
 * A094201  a(n) = 4*n^5 + 10*n^4 + 13*n^3 + 11*n^2 + 5*n + 1.
 * https://oeis.org/A094201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^5 + 10*n^4 + 13*n^3 + 11*n^2 + 5*n + 1
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t to avoid overflow of intermediate integer types. */
    seq_t tn = (seq_t)n;

    /* Compute successive powers of n and accumulate the polynomial. */
    seq_t n2 = tn * tn;                 /* n^2          */
    seq_t n3 = n2 * tn;                 /* n^3          */
    seq_t n4 = n3 * tn;                 /* n^4          */
    seq_t n5 = n4 * tn;                 /* n^5          */

    seq_t result = 0;
    result += 4 * n5;   /* 4*n^5  */
    result += 10 * n4;  /* +10*n^4*/
    result += 13 * n3;  /* +13*n^3*/
    result += 11 * n2;  /* +11*n^2*/
    result += 5 * tn;   /* +5*n   */
    result += 1;        /* +1     */

    return result;
}
