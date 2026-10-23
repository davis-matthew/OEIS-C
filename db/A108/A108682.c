/*
 * A108682  a(n) = (n+1)*(n+2)^2*(n+3)*(n+4)*(4*n^2+15*n+15)/720.
 * https://oeis.org/A108682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)^2*(n+3)*(n+4)*(4*n^2+15*n+15)/720
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for intermediate products to avoid overflow. */
    unsigned __int128 x = (unsigned __int128)n;
    unsigned __int128 prod = 1;

    prod *= (x + 1);                               /* (n+1)          */
    prod *= (x + 2);                               /* (n+2)          */
    prod *= (x + 2);                               /* (n+2) again    */
    prod *= (x + 3);                               /* (n+3)          */
    prod *= (x + 4);                               /* (n+4)          */
    prod *= (4*x*x + 15*x + 15);                   /* 4*n^2+15*n+15  */
    prod /= 720;                                   /* divide by 720   */

    return (seq_t)prod;
}
