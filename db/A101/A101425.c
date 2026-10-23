/*
 * A101425  Number of different cuboids with volume p^5 X q^n, where p,q are
 *          distinct prime numbers.
 * https://oeis.org/A101425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (37 + 3*(-1)^n + 48*n + 14*n^2)/8
 * Range:     u8 n <= 2, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to seq_t once for arithmetic */
    seq_t nn = (seq_t)n;
    /* compute numerator differently for odd/even n to avoid signed arithmetic */
    seq_t num;
    if (n & 1)  /* n odd: (-1)^n = -1, 37 + 3*(-1) = 34 */
        num = (seq_t)34 + (seq_t)48 * nn + (seq_t)14 * nn * nn;
    else        /* n even: (-1)^n =  1, 37 + 3* 1 = 40 */
        num = (seq_t)40 + (seq_t)48 * nn + (seq_t)14 * nn * nn;
    return num / (seq_t)8;
}
