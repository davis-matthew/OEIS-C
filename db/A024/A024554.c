/*
 * A024554  a(n) = Sum_{k=1..n} floor( 1/{k*sqrt(5)} ), where {x} := x -
 *          floor(x).
 * https://oeis.org/A024554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(1/{k*sqrt(5)})
 * Range:     u8 n <= 7, u16 n <= 113, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the identity:
 *   for k≥1,
 *   let s = floor(sqrt(5*k^2)), D = 5*k^2 - s^2,
 *   then floor(1/{k*sqrt(5)}) = floor(2*s / D)
 *   (s, D are integers, D>0). */
#include <stdint.h>

/* Integer square root of a 64‑bit unsigned integer. */
static inline uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t low = 0;
    uint64_t high = (x < ((uint64_t)1 << 32)) ? x : ((uint64_t)1 << 32);
    uint64_t r = 0;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        uint64_t prod = (unsigned __int128)mid * mid;
        if (prod <= x) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return r;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t ksq = (seq_t)k * (seq_t)k;           /* k^2   */
        seq_t val = 5 * ksq;                      /* 5*k^2 */
        /* val fits in 64 bits for n up to ~10^9; safe for the
           expected limits (n≈10 000). */
        uint64_t val64 = (uint64_t)val;
        uint64_t s = isqrt_uint64(val64);        /* floor(sqrt(5*k^2)) */
        seq_t s_sq = (seq_t)s * (seq_t)s;        /* s^2   */
        seq_t D = val - s_sq;                    /* 5*k^2 - s^2 > 0 */
        seq_t term = ((seq_t)s << 1) / D;        /* floor(2*s / D) */
        sum += term;
    }
    return sum;
}
