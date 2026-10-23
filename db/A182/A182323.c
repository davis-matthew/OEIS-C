/*
 * A182323  a(n) = (194*n + 3*(-1)^n + 1)/4 + 24.
 * https://oeis.org/A182323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (194*n + 3*(-1)^n + 1)/4 + 24, offset 0
 * Range:     u8 n <= 1, u16 n <= 365, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert index to seq_t for arithmetic */
    seq_t nn = (seq_t)n;
    seq_t num;

    /* handle the +/-3 term without negative numbers */
    if ((n & 1ULL) == 0)          /* n even: (-1)^n = +1 */
    {
        num = 194 * nn + 4;       /* 3*+1 + 1 = 4  */
    }
    else                          /* n odd : (-1)^n = -1 */
    {
        num = 194 * nn - 2;       /* 3*-1 + 1 = -2 */
    }

    /* divide by 4 (exact division) and add the constant 24 */
    return num / 4 + 24;
}
