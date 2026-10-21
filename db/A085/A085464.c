/*
 * A085464  Number of monotone n-weightings of complete bipartite digraph
 *          K(4,2).
 * https://oeis.org/A085464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(2*n^4+4*n^3+6*n^2+4*n-1)/30
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 62, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not needed for offset 1, but safe */

    /* use 128‑bit arithmetic for the intermediate product to avoid overflow */
    unsigned __int128 n128 = (unsigned __int128)n;
    unsigned __int128 n1   = n128 + 1;

    /* compute n^2 , n^3 , n^4 using 128‑bit */
    unsigned __int128 n2   = n128 * n128;
    unsigned __int128 n3   = n2 * n128;
    unsigned __int128 n4   = n3 * n128;

    /* polynomial part: 2*n^4 + 4*n^3 + 6*n^2 + 4*n - 1 */
    unsigned __int128 poly = 2 * n4 + 4 * n3 + 6 * n2 + 4 * n128 - 1;

    /* complete product and division by 30 */
    unsigned __int128 prod = n128 * n1 * poly;
    prod /= 30;

    return (seq_t)prod;
}
