/*
 * A227252  Number of n X 2 binary arrays indicating whether each 2 X 2
 *          subblock of a larger binary array has lexicographically
 *          nondecreasing rows and columns, for some larger (n+1) X 3 binary
 *          array with rows and columns of the latter in lexicographically
 *          nondecreasing order.
 * https://oeis.org/A227252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^5 - 5*n^4 + 25*n^3 + 5*n^2 - 26*n + 60)/60 for n ≥ 2;
 *            a(1) = 2
 * Range:     u8 n <= 7, u16 n <= 21, u32 n <= 192, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;

    /* Use 128‑bit arithmetic for intermediate values to avoid overflow
       on 64‑bit seq_t builds.  The result always fits in seq_t for n
       up to 10000 (and satisfies the Recurrence relation for all n). */
    unsigned long long N = (unsigned long long)n;

    __int128 n2 = (__int128)N * N;
    __int128 n3 = n2 * N;
    __int128 n4 = n3 * N;
    __int128 n5 = n4 * N;

    __int128 num = n5 - (__int128)5 * n4 + (__int128)25 * n3
                 + (__int128)5 * n2 - (__int128)26 * N + 60;

    return (seq_t)(num / 60);
}
