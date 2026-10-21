/*
 * A055841  Number of compositions of n into 3*j-1 kinds of j's for all j >=
 *          1.
 * https://oeis.org/A055841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1; a(1)=2; a(n)=9*4^(n-2) for n>=2 (equivalently
 *            a(n)=4*a(n-1) for n>=3)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;
    /* For n >= 2 the recurrence a(n) = 4 * a(n-1) holds from n = 3 onward.
       For n = 2 the value is 9, which is the base of the geometric progression. */
    seq_t result = (seq_t)9;   /* a(2) */
    idx_t i;

    for (i = 3; i <= n; ++i)
        result = result * (seq_t)4;   /* multiply by 4 each step */

    return result;
}
