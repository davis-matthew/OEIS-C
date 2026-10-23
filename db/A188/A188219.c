/*
 * A188219  Positions of 0 in the zero-one sequence [n*r]-[4*r]-[n*r-4*r],
 *          where r=sqrt(5), n>=1.
 * https://oeis.org/A188219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer k (k >= 4) for which floor(k*sqrt(5)) -
 *            floor((k-4)*sqrt(5)) == 8. The equality holds exactly when the
 *            4‑step increment of floor(m*sqrt(5)) equals 8. Since
 *            floor(m*sqrt(5)) = floor(sqrt(5*m*m)), we can test it using
 *            integer square roots only
 * Range:     u8 n <= 15, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for 0 <= x < 2^64.  */
static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = 4294967295ULL;              /* 2^32-1 is max sqrt(2^64-1) */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* out of range, safety */
    idx_t count = 0;
    for (uint64_t k = 4; ; ++k) {
        uint64_t val_k   = isqrt_u64(5ULL * k * k);
        uint64_t km4     = k - 4;
        uint64_t val_km4 = isqrt_u64(5ULL * km4 * km4);
        if (val_k - val_km4 == 8ULL) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
    }
}
