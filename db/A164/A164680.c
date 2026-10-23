/*
 * A164680  Expansion of x/((1-x)^3*(1-x^2)^3*(1-x^3)).
 * https://oeis.org/A164680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round( -(-1)^n*(n+3)*(n+7)/256 + (6*n^6 +180*n^5
 *            +2070*n^4 +11400*n^3 +30429*n^2 +34290*n +9785)/103680 )
 * Range:     u8 n <= 8, u16 n <= 27, u32 n <= 200, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Use signed __int128 for intermediate calculations to avoid overflow.
       All intermediate values fit comfortably within 128 bits for n up to 10⁴. */
    __int128 nn = n;                     /* make a signed 128–bit copy of n          */
    __int128 n2 = nn * nn;               /* n²                                        */
    __int128 n3 = n2 * nn;               /* n³                                        */
    __int128 n4 = n3 * nn;               /* n⁴                                        */
    __int128 n5 = n4 * nn;               /* n⁵                                        */
    __int128 n6 = n5 * nn;               /* n⁶                                        */

    /* Polynomial part / 103680 (high‑degree term first for readability) */
    __int128 poly = 6   * n6
                  + 180 * n5
                  + 2070 * n4
                  + 11400 * n3
                  + 30429 * n2
                  + 34290 * nn
                  + 9785;                /* constant term                           */

    /* -(-1)^n*(n+3)*(n+7) part multiplied by 405 (since 103680/256 = 405)   */
    __int128 sign = (nn & 1) ? 1 : -1;    /* 1 for odd n, -1 for even n             */
    __int128 term1 = sign * (nn + 3) * (nn + 7) * 405;

    /* Total numerator with common denominator 103680 */
    __int128 numerator = poly + term1;   /* numerator will be positive for n >= 1  */

    /* Rounding: nearest integer -> add half the denominator before truncating */
    __int128 result128 = (numerator + 51840) / 103680; /* 0.5 * 103680 = 51840 */

    return (seq_t)result128;             /* cast back to the build's seq_t type    */
}
