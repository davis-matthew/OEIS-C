/*
 * A212841  Number of 0..n arrays of length 8 with 0 never adjacent to n.
 * https://oeis.org/A212841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^8 + 8*n^7 + 14*n^6 - 16*n^5 - 30*n^4 + 24*n^3 + 8*n^2 -
 *            8*n + 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to seq_t once to avoid repeated conversions */
    seq_t x = (seq_t)n;
    /* compute successive powers of n */
    seq_t x2 = x * x;          /* n^2 */
    seq_t x3 = x2 * x;         /* n^3 */
    seq_t x4 = x3 * x;         /* n^4 */
    seq_t x5 = x4 * x;         /* n^5 */
    seq_t x6 = x5 * x;         /* n^6 */
    seq_t x7 = x6 * x;         /* n^7 */
    seq_t x8 = x7 * x;         /* n^8 */

    /* evaluate the polynomial */
    seq_t res = x8;                        /* n^8 */
    res += (seq_t)8 * x7;                  /* + 8*n^7 */
    res += (seq_t)14 * x6;                 /* + 14*n^6 */
    res -= (seq_t)16 * x5;                 /* - 16*n^5 */
    res -= (seq_t)30 * x4;                 /* - 30*n^4 */
    res += (seq_t)24 * x3;                 /* + 24*n^3 */
    res += (seq_t)8 * x2;                  /* + 8*n^2 */
    res -= (seq_t)8 * x;                   /* - 8*n */
    res += (seq_t)1;                       /* + 1 */

    return res;
}
