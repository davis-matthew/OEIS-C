/*
 * A188510  Expansion of x*(1 + x^2)/(1 + x^4) in powers of x.
 * https://oeis.org/A188510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = 0 if n%2==0, else a(n) = ((n/4)%2==0 ? 1 : -1)
 * Range:     i8 n <= 2500, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for even indices */
    if ((n & 1) == 0)
        return (seq_t)0;

    /* For odd indices, the value alternates every two odd powers:
       a(4k+1)=a(4k+3)=(-1)^k   */
    if (((n / 4) & 1) == 0) /* k even => (+1) */
        return (seq_t)1;
    else /* k odd => (-1) */
        return (seq_t)-1;
}
