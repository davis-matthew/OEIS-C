/*
 * A067752  Number of unordered solutions of xy + xz + yz = n in nonnegative
 *          integers.
 * https://oeis.org/A067752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of unordered solutions (x,y,z) ≥ 0 to xy + xz + yz
 *            = n. The algorithm enumerates all triples with x ≤ y ≤ z and
 *            counts them
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* integer square root for 64‑bit unsigned values */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = ((uint64_t)1 << 32);
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if ((__int128)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* count unordered triples (x,y,z)>=0 solving xy+xz+yz = n, n>=1 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not in OEIS offset, but safe */

    uint64_t N = n;                        /* promote for 64‑bit ops */
    uint64_t cnt = 0;

    /* x = 0 case: y*z = n, with 0 < y ≤ z.  Count divisor pairs. */
    uint64_t root = isqrt_u64(N);
    for (uint64_t d = 1; d <= root; ++d) {
        if (N % d == 0) {
            uint64_t e = N / d;
            if (d <= e) cnt++;            /* unordered pair (d, e) */
        }
    }

    /* x >= 1 case.  Enumerate x <= y <= z. */
    uint64_t xmax = isqrt_u64(N);          /* x cannot exceed sqrt(n) */
    for (uint64_t x = 1; x <= xmax; ++x) {
        uint64_t ymax = N / x;             /* because x*y ≤ n */
        for (uint64_t y = x; y <= ymax; ++y) {
            uint64_t xy = x * y;
            if (xy > N) break;            /* safety */
            uint64_t rhs = N - xy;
            uint64_t denom = x + y;
            if (rhs % denom != 0) continue;
            uint64_t z = rhs / denom;
            if (z < y) break;             /* further y will make z even smaller */
            /* now x <= y <= z and equation satisfied */
            cnt++;
        }
    }

    return (seq_t)cnt;
}
