/*
 * A194153  a(n) = Sum_{j=1..n} floor(j*(sqrt(5))/2); n-th partial sum of
 *          Beatty sequence for (sqrt(5))/2.
 * https://oeis.org/A194153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = ∑_{j=1..n} ⌊j·√5⁄2⌋
 *
 * The computation uses only integer arithmetic.
 * For each j we evaluate ⌊j·√5⌋ = ⌊√(5·j²)⌋ and then divide by 2.
 */

#include <stdint.h>

/* integer square root for 64‑bit unsigned values */
static inline uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 32;         /* hi > sqrt(UINT64_MAX) */
    while (lo < hi) {
        uint64_t mid = (lo + hi) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;     /* prevent overflow */
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t j = 1; j <= n; ++j) {
        uint64_t j64   = (uint64_t)j;
        uint64_t val   = 5 * j64 * j64;   /* 5·j², fits in 64 bits for n≤10 000 */
        uint64_t root  = isqrt_uint64(val);       /* ⌊√(5·j²)⌋ == ⌊j·√5⌋   */
        sum += (seq_t)(root / 2); /* floor division by 2 gives ⌊j·√5⁄2⌋ */
    }
    return sum;
}
