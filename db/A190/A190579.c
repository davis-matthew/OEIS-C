/*
 * A190579  Number of ways to place 6 nonattacking grasshoppers on an n x n
 *          chessboard.
 * https://oeis.org/A190579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^12/720 - n^10/48 -5*n^9/9 +509*n^8/144 -187*n^7/90 *
 *            +701*n^6/48 -14467*n^5/36 +666917*n^4/360 -471121*n^3/180 *
 *            -59875*n^2/24 +57101*n/6 -11339/2 -(9*n^2/8-n-7/2)*(-1)^n, * for
 *            n > 5 (a(1)=0, a(2)=0, a(3)=2, a(4)=998, a(5)=51618)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 69, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the few small values that are not covered by the formula  */
    if (n <= 5) {
        switch (n) {
            case 1: return 0;
            case 2: return 0;
            case 3: return 2;
            case 4: return 998;
            case 5: return 51618;
            default: break;
        }
    }

    /* use a wider type for intermediate calculations */
#if defined(__SIZEOF_INT128__)
    typedef unsigned __int128 wide_t;
#else
    /* Fallback: for compilers without __int128 simply use a big‑int type
       (assumed to be usable with the arithmetic operators). */
    typedef seq_t wide_t;
#endif

    /* pre‑compute powers of n up to n^12                */
    wide_t pw[13];
    pw[0] = (wide_t)1;
    wide_t w_n = (wide_t)n;
    for (idx_t i = 1; i <= 12; ++i)
        pw[i] = pw[i - 1] * w_n;

    /* accumulate the polynomial part of the numerator    */
    wide_t num = 0;
    num += pw[12] * (wide_t)1;
    num -= pw[10] * (wide_t)15;
    num -= pw[9]  * (wide_t)400;
    num += pw[8]  * (wide_t)2545;
    num -= pw[7]  * (wide_t)1496;
    num += pw[6]  * (wide_t)10515;
    num -= pw[5]  * (wide_t)289340;
    num += pw[4]  * (wide_t)1333834;
    num -= pw[3]  * (wide_t)1884484;
    num -= pw[2]  * (wide_t)1796250;
    num += pw[1]  * (wide_t)6852120;
    num -= (wide_t)4082040;

    /* alternating part:  -(9*n^2/8 - n - 7/2)*(-1)^n   */
    wide_t alt = (wide_t)810 * pw[2] - (wide_t)720 * w_n - (wide_t)2520;
    if ((n & (idx_t)1) == 0)
        num -= alt;          /* even n  : subtract                     */
    else
        num += alt;          /* odd  n  : add                          */

    /* final division by 720 – the result fits into seq_t       */
    return (seq_t)(num / (wide_t)720);
}
