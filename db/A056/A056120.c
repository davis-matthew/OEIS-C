/*
 * A056120  a(n) = (3^3)*4^(n-3) with a(0)=1, a(1)=1 and a(2)=7.
 * https://oeis.org/A056120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 27 * 4^(n-3) for n>=3; a(0)=1, a(1)=1, a(2)=7
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the special initial values explicitly */
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 7;

    /* For n >= 3:  a(n) = 27 * 4^(n-3) = 27 * 2^(2*(n-3)) */
    idx_t exp = n - 3;                 /* exponent for 4 (i.e., n-3) */
    seq_t result = 27;                  /* start with the constant 27 */
    result <<= (2 * exp);               /* multiply by 2^(2*(n-3)) = 4^(n-3) */
    return result;
}
