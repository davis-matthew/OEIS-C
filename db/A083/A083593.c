/*
 * A083593  Expansion of 1/((1-2*x)*(1-x^4)).
 * https://oeis.org/A083593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1; for n>0, a(n)=2*a(n-1)+f(n) where f(n)=1 if n%4==0,
 *            otherwise 0
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t val = (seq_t)1;             /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        val = val << 1;               /* multiply by 2 */
        if ((i & 3U) == 0U)           /* check i % 4 == 0 */
            ++val;                     /* add 1 for multiples of 4 */
    }
    return val;
}
