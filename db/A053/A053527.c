/*
 * A053527  Number of bipartite graphs with 4 edges on nodes {1..n}.
 * https://oeis.org/A053527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-3)*(n-2)*(n-1)*n*(n+2)*(n^3-5*n-36)/384 with a(n)=0
 *            for n<4
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 34, u64 n <= 539, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)0;

    unsigned __int128 N = (unsigned __int128)n;

    /* Compute (n-3)*(n-2)*(n-1)*n*(n+2) */
    unsigned __int128 part1 = (N - 3) * (N - 2) * (N - 1) * N * (N + 2);

    /* Compute n^3 - 5*n - 36 */
    unsigned __int128 part2 = N * N * N - 5 * N - 36;

    /* Multiply, divide by 384 */
    unsigned __int128 prod = part1 * part2;
    prod /= 384;

    return (seq_t)prod;
}
