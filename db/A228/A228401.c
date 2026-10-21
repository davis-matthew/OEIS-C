/*
 * A228401  The number of permutations of length n sortable by 2 block
 *          interchanges.
 * https://oeis.org/A228401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + n*(n-1)*(n+1)*(n+2)*(3*n^4 - 10*n^3 - 11*n^2 + 50*n +
 *            216)/5760
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 41, u64 n <= 658,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* unused; offset is 1 */
    __int128 N = (__int128)n;
    __int128 p = 3*N*N*N*N - 10*N*N*N - 11*N*N + 50*N + 216;
    __int128 prod = N * (N-1) * (N+1) * (N+2) * p;
    __int128 res = prod / 5760 + 1;
    return (seq_t)res;
}
