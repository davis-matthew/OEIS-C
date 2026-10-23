/*
 * A194161  a(n) = [Sum_{k=1..n} (k*r)], where [ ]=floor, ( )=fractional part,
 *          and r=sqrt(2).
 * https://oeis.org/A194161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n(n+1) * sqrt(2) / 2) - Σ_{k=1..n} floor(k *
 *            sqrt(2))
 * Range:     u8 n <= 512, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*        = floor(n(n+1) * sqrt(2) / 2) - Σ_{k=1..n} floor(k * sqrt(2))            */
/* where sqrt(2) ≈ 1.414213562373095, r = √2.                                              */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for 0 <= x <= UINT64_MAX */
static unsigned long long isqrt_u64(unsigned long long x) {
    unsigned long long r = 0;
    unsigned long long bit = 1ULL << 62;           /* largest power of 4 <= x */
    while (bit > x) bit >>= 2;
    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;        /* offset is 1; 0 is unused but safe */
    seq_t sum_floor = 0;

    /* sum_{k=1..n} floor(k * sqrt(2)) */
    for (idx_t k = 1; k <= n; ++k) {
        unsigned long long kk = (unsigned long long)k;
        unsigned long long v = 2ULL * kk * kk;      /* 2 * k^2 */
        unsigned long long fl = isqrt_u64(v);       /* floor(k * sqrt(2)) */
        sum_floor += (seq_t)fl;
    }

    /* floor(n(n+1) * sqrt(2) / 2)  = floor(sqrt(2 * T^2))  where T = n(n+1)/2 */
    unsigned long long nn = (unsigned long long)n;
    unsigned long long T  = nn * (nn + 1) / 2;      /* n(n+1)/2 */
    unsigned long long vT = 2ULL * T * T;           /* 2 * T^2 */
    seq_t floor_rT = (seq_t)isqrt_u64(vT);          /* floor(n(n+1) * sqrt(2) / 2) */

    return floor_rT - sum_floor;
}
