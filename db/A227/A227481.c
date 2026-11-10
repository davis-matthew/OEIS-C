/*
 * A227481  Number of squares in row n of the triangle in A069011.
 * https://oeis.org/A227481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum(A010052(A069011(n,k)): k=0..n), i.e. the number of k
 *            in 0..n with n^2 + k^2 a perfect square
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 102 terms through n = 998 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    unsigned __int128 nn = (unsigned __int128)n * n;
    unsigned __int128 m = n;          /* smallest m with m^2 >= n^2 + k^2, nondecreasing in k */
    for (idx_t k = 0; k <= n; k++) {
        unsigned __int128 t = nn + (unsigned __int128)k * k;
        while (m * m < t) m++;
        if (m * m == t) count++;
        if (k == n) break;
    }
    return count;
}
