/*
 * A188079  a(n) = [n*r]-[k*r]-[n*r-k*r], where r=sqrt(3), k=6, [ ]=floor.
 * https://oeis.org/A188079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*sqrt(3)) - 10 - floor((n-6)*sqrt(3))
 * Range:     u8 n <= 130, u16 n <= 130, u32 n <= 130, u64 n <= 130,
 *            u128 n <= 130, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root of a 64‑bit unsigned integer */
static inline seq_t isqrt_uint64(uint64_t v)
{
    uint64_t lo = 0, hi = v;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == v) return (seq_t)mid;
        if (sq < v) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return (seq_t)(lo - 1);
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset protection */

    /* floor(n*sqrt(3)) */
    uint64_t v_n = 3ULL * n * n;
    seq_t floor_n = isqrt_uint64(v_n);

    /* floor((n-6)*sqrt(3)) */
    int64_t floor_n6;
    if (n >= 6) {
        uint64_t v_n6 = 3ULL * (n - 6) * (n - 6);
        floor_n6 = (int64_t)isqrt_uint64(v_n6);
    } else {
        uint64_t v_n6 = 3ULL * (6 - n) * (6 - n);
        seq_t pos = isqrt_uint64(v_n6);
        floor_n6 = -((int64_t)pos + 1);
    }

    /* compute a(n) */
    int64_t result = (int64_t)floor_n - 10 - floor_n6;
    return (seq_t)result;
}
