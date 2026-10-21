/*
 * A212673  Number of (w,x,y,z) with all terms in {1,...,n} and
 *          w<=|x-y|+|y-z|.
 * https://oeis.org/A212673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-1+(-1)^n-8*n^2+10*n^4)/16
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t t2 = tn * tn;       /* n^2 */
    seq_t t4 = t2 * t2;       /* n^4 */
    seq_t num = 10 * t4 - 8 * t2;   /* 10*n^4 - 8*n^2 */
    if ((n & 1) != 0)         /* odd n: subtract 2 */
        num -= 2;
    return num / 16;          /* divide by 16 */
}
