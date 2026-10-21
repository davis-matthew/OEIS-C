/*
 * A063757  Expansion of g.f. (1+3*x+2*x^2)/((1-x)*(1-2*x^2)).
 * https://oeis.org/A063757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1,a(1)=4,a(2)=8,a(n)=a(n-1)+2*a(n-2)-2*a(n-3) Equivalent
 *            closed form: a(2k)=7*2^k-6, a(2k+1)=10*2^k-6
 * Range:     u8 n <= 10, u16 n <= 26, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 2^{floor(n/2)} */
    idx_t exp = n >> 1;          /* n/2, integer floor */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < exp; ++i)
        pow2 *= 2;               /* multiply by 2 repeatedly */

    if ((n & 1) == 0)            /* even index: a(2k) */
        return 7 * pow2 - 6;
    else                         /* odd index: a(2k+1) */
        return 10 * pow2 - 6;
}
