/*
 * A049759  Triangular array T read by rows: T(n,k)=n^2 mod k, for
 *          k=1,2,...,n, n=1,2,...
 * https://oeis.org/A049759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = row^2 mod col, where row = smallest integer with
 *            row*(row+1)/2 >= n, col = n - (row-1)*row/2
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t N = n;

    /* Find row r such that r*(r+1)/2 >= N */
    uint64_t lo = 1, hi = 1;
    while (( (__int128)hi * (hi + 1) / 2 ) < N)
        hi <<= 1;

    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (( (__int128)mid * (mid + 1) / 2 ) >= N)
            hi = mid;
        else
            lo = mid + 1;
    }
    uint64_t r = lo;

    /* Compute column k */
    uint64_t triBefore = ( (__int128)(r - 1) * r ) / 2;
    uint64_t k = N - triBefore;

    /* r^2 mod k */
    __int128 prod = (__int128)r * r;
    uint64_t rem = (uint64_t)(prod % k);
    return (seq_t)rem;
}
