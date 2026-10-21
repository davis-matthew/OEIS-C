/*
 * A056078  Number of proper T_1-hypergraphs with 3 labeled nodes and n
 *          hyperedges.
 * https://oeis.org/A056078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-240+394*n-135*n^2-35*n^3+15*n^4+n^5)/120
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 217, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Use signed __int128 to avoid overflow of intermediate values
       up to n^5 (~1e20 for n=10000). */
    __int128 x = n;
    __int128 s = x + 15;              /* (n + 15)          */
    s = s * x - 35;                   /* ((n + 15)*n - 35) */
    s = s * x - 135;                  /* (((n + 15)*n - 35)*n - 135) */
    s = s * x + 394;                  /* ((((n + 15)*n - 35)*n - 135)*n + 394) */
    s = s * x - 240;                  /* (((((n + 15)*n - 35)*n - 135)*n + 394)*n - 240) */

    return (seq_t)(s / 120);          /* divide by 120, exact */
}
