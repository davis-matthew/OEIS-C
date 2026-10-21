/*
 * A084351  Length of period of sequences
 *          r(k,n)=floor(sin(1)*k!)-n*floor(sin(1)*k!/n) when n is fixed.
 * https://oeis.org/A084351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   for n > 1, a(n) = n if n≡0 (mod 4), a(n) = 2n if n≡2 (mod 4),
 *            a(n) = 4n if n is odd. For n = 1, a(1) = 1
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the special case a(1) = 1 */
    if (n == 1)
        return (seq_t)1;

    /* n > 1: use the piecewise formula */
    if ((n & 3) == 0)         /* n divisible by 4 */
        return (seq_t)n;
    else if ((n & 1) == 0)    /* n even but not divisible by 4 (i.e., n ≡ 2 mod 4) */
        return (seq_t)n * (seq_t)2;
    else                      /* n odd */
        return (seq_t)n * (seq_t)4;
}
