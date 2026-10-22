/*
 * A003056  n appears n+1 times. Also the array A(n,k) = n+k (n >= 0, k >= 0)
 *          read by antidiagonals. Also inverse of triangular numbers.
 * https://oeis.org/A003056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, tabl
 * Formula:   a(n) = k if k*(k+1)/2 <= n < (k+1)*(k+2)/2, i.e. a(n) =
 *            floor((sqrt(1+8*n)-1)/2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 33;
    unsigned __int128 N = (unsigned __int128)n;
    /* largest k with k*(k+1)/2 <= n; T(0) = 0 <= n always, T(2^33) > 2^64 > n */
    while (hi - lo > 1) {
        uint64_t mid = lo + (hi - lo) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= N)
            lo = mid;
        else
            hi = mid;
    }
    return (seq_t)lo;
}
