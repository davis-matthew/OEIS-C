/*
 * A008390  Crystal ball sequence for A_7 lattice.
 * https://oeis.org/A008390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            143/210*n^7+143/60*n^6+451/60*n^5+77/6*n^4+937/60*n^3+707/60*n^2+363/70*n+1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 24, u64 n <= 596, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute the sum with common denominator 420 to avoid truncation */
    unsigned __int128 sn   = (unsigned __int128)n;
    unsigned __int128 p2   = sn * sn;          /* n^2   */
    unsigned __int128 p3   = p2 * sn;          /* n^3   */
    unsigned __int128 p4   = p3 * sn;          /* n^4   */
    unsigned __int128 p5   = p4 * sn;          /* n^5   */
    unsigned __int128 p6   = p5 * sn;          /* n^6   */
    unsigned __int128 p7   = p6 * sn;          /* n^7   */

    unsigned __int128 num = 0;
    num += (unsigned __int128)286  * p7;   /* 143/210 = 286/420 */
    num += (unsigned __int128)1001 * p6;   /* 143/60  = 1001/420 */
    num += (unsigned __int128)3157 * p5;   /* 451/60  = 3157/420 */
    num += (unsigned __int128)5390 * p4;   /* 77/6    = 5390/420 */
    num += (unsigned __int128)6559 * p3;   /* 937/60  = 6559/420 */
    num += (unsigned __int128)4949 * p2;   /* 707/60  = 4949/420 */
    num += (unsigned __int128)2178 * sn;    /* 363/70  = 2178/420 */
    num += (unsigned __int128)420;         /* constant 1 = 420/420 */

    seq_t result = (seq_t)(num / 420);
    return result;
}
