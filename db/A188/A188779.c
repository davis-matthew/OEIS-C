/*
 * A188779  Number of 4-turn bishop's tours on an n X n board summed over all
 *          starting positions.
 * https://oeis.org/A188779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -2 + 257*n/15 - 109*n^2/3 + 106*n^3/3 - 47*n^4/3 +
 *            38*n^5/15 + (-1)^n*(2-n) (n >= 1)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* below the offset (n<1) is undefined */

    /* Use a signed 128‑bit integer to hold intermediate results.
       The expression is always divisible by 15, so the division is exact. */
    __int128 nn = n;
    __int128 num = -30;                 /* -2 * 15                 */

    num += (__int128)257 * nn;           /* 257*n                     */
    num -= (__int128)545 * nn * nn;      /* -109*n^2 * 5              */
    num += (__int128)530 * nn * nn * nn; /* 106*n^3 * 5               */
    num -= (__int128)235 * nn * nn * nn * nn; /* -47*n^4 * 5         */
    num += (__int128)38 * nn * nn * nn * nn * nn; /* 38*n^5          */

    if ((n & 1ULL) == 0)                 /* (-1)^n = +1 for even n   */
        num += (__int128)15 * ( (__int128)2 - nn );
    else                                 /* (-1)^n = -1 for odd  n   */
        num += (__int128)15 * ( nn - (__int128)2 );

    /* 15*a(n) is stored in 'num' – divide to obtain the result. */
    return (seq_t)(num / 15);
}
