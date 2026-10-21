/*
 * A064860  Period of continued fraction for sqrt(7)*n.
 * https://oeis.org/A064860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 76, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Period of continued fraction of sqrt(7)*n */
#include <stdint.h>

/* Integer square root for 64‑bit numbers (binary search, 128‑bit intermediate) */
static seq_t isqrt64(uint64_t x)
{
    uint64_t lo = 0, hi = x, mid, ans = 0;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        if ((__uint128_t)mid * mid <= x) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return (seq_t)ans;
}

seq_t a(idx_t n)
{
    /* N = 7 * n^2 (always fits in 64 bits for n <= 10000) */
    uint64_t un = (uint64_t)n;
    uint64_t N64 = 7ULL * un * un;           /* 64‑bit integer */
    seq_t N = (seq_t)N64;                    /* type‑safe cast */

    seq_t a0 = isqrt64(N64);                 /* floor(sqrt(N)) */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a = a0;
    uint64_t cnt = 0;                        /* period counter fits in 64 bits */

    do {
        m = d * a - m;
        d = (N - m * m) / d;
        a = (a0 + m) / d;
        cnt++;
    } while (a != 2 * a0);

    return (seq_t)cnt;
}
