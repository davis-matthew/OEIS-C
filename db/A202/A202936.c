/*
 * A202936  Number of (n+3) X 8 binary arrays with consecutive windows of four
 *          bits considered as a binary number nondecreasing in every row and
 *          column.
 * https://oeis.org/A202936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/1680)*n^8 + (17/210)*n^7 + (134/45)*n^6 + (256/5)*n^5
 *            + (352481/720)*n^4 + (55381/20)*n^3 + (32201731/2520)*n^2 +
 *            (19670981/420)*n + 97073 (Computes the number of (n+3) × 8
 *            binary arrays with consecutive windows of four bits sorted
 *            nondecreasingly in rows and columns.)
 * Range:     u8 none, u16 none, u32 n <= 28, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for index 0 */

    /* use 128‑bit arithmetic for safe intermediate products */
    unsigned __int128 N = n;        /* n as 128‑bit unsigned */

    /* compute powers of n up to n^8 */
    unsigned __int128 p1 = N;          /* n^1 */
    unsigned __int128 p2 = p1 * N;     /* n^2 */
    unsigned __int128 p3 = p2 * N;     /* n^3 */
    unsigned __int128 p4 = p3 * N;     /* n^4 */
    unsigned __int128 p5 = p4 * N;     /* n^5 */
    unsigned __int128 p6 = p5 * N;     /* n^6 */
    unsigned __int128 p7 = p6 * N;     /* n^7 */
    unsigned __int128 p8 = p7 * N;     /* n^8 */

    /* build the numerator of the rational expression */
    unsigned __int128 num = 
          (unsigned __int128)3      * p8   /* 3*n^8 */
        + (unsigned __int128)408    * p7   /* 408*n^7 */
        + (unsigned __int128)15008  * p6   /* 15008*n^6 */
        + (unsigned __int128)258048 * p5   /* 258048*n^5 */
        + (unsigned __int128)2467367* p4   /* 2467367*n^4 */
        + (unsigned __int128)13956012* p3  /* 13956012*n^3 */
        + (unsigned __int128)64403462* p2  /* 64403462*n^2 */
        + (unsigned __int128)236051772* p1 /* 236051772*n  */
        + (unsigned __int128)489247920;    /* constant term */

    /* denominator is 5040 (lcm of all denominators) */
    unsigned __int128 res128 = num / 5040;

    return (seq_t)res128;
}
