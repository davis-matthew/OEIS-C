/*
 * A099430  a(n) = 2^n+(-1)^n-1.
 * https://oeis.org/A099430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n + (-1)^n - 1, Offset: 0
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow2 = 1;                /* 2^0 */
    for (idx_t i = 0; i < n; ++i)  /* compute 2^n by repeated doubling */
        pow2 <<= 1;                /* safe for all unsigned seq_t types */

    if ((n & 1) == 0)             /* if n even, (-1)^n = +1 */
        pow2 += 1;
    else                          /* if n odd,  (-1)^n = -1 */
        pow2 -= 1;

    pow2 -= 1;                     /* subtract the final 1 in the formula */
    return pow2;
}
