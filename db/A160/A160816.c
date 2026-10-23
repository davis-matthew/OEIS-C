/*
 * A160816  Expansion of (1+62*x+563*x^2+1032*x^3+467*x^4+50*x^5+x^6)/(1-x)^7.
 * https://oeis.org/A160816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 136*n^6/45 +151*n^5/15 +167*n^4/9 +19*n^3+559*n^2/45
 *            +74*n/15 +1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 32, u64 n <= 1351,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a common denominator 45, compute the numerator in a wide type
       to avoid overflow in 64‑bit builds, and then divide. */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 n2 = nn * nn;          /* n^2 */
    unsigned __int128 n3 = n2 * nn;          /* n^3 */
    unsigned __int128 n4 = n3 * nn;          /* n^4 */
    unsigned __int128 n5 = n4 * nn;          /* n^5 */
    unsigned __int128 n6 = n5 * nn;          /* n^6 */

    unsigned __int128 num
        = (unsigned __int128)136 * n6          /* 136*n^6 */
        + (unsigned __int128)453 * n5          /* 151*n^5*3/45 = 453*n^5/45 */
        + (unsigned __int128)835 * n4          /* 167*n^4*5/45 = 835*n^4/45 */
        + (unsigned __int128)855 * n3          /* 19*n^3*45/45 = 855*n^3/45 */
        + (unsigned __int128)559 * n2          /* 559*n^2/45 */
        + (unsigned __int128)222 * nn          /* 74*n*3/45 = 222*n/45 */
        + (unsigned __int128)45;              /* 1*45/45 */

    return (seq_t)(num / 45);
}
