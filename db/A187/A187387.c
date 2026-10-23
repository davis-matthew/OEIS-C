/*
 * A187387  Floor(r*n), where r=1+sqrt(6)+sqrt(5); complement of A187388.
 * https://oeis.org/A187387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((1 + sqrt(6) + sqrt(5)) * n)
 * Range:     u8 n <= 45, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root for 64‑bit values */
static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 32;
    while (lo + 1 < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if ((__uint128_t)mid * mid <= x)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1; guard against 0 */
    uint64_t nn = (uint64_t)n;

    /* floor(n*sqrt(6)) and floor(n*sqrt(5)) via integer sqrt */
    uint64_t s6 = isqrt_u64(6ULL * nn * nn);
    uint64_t s5 = isqrt_u64(5ULL * nn * nn);

    /* base integer part */
    uint64_t base = nn + s6 + s5;

    /* inequality test: 120*n^4 >= ( (s6+s5+1)^2 - 11*n^2 )^2  */
    __uint128_t n2 = (__uint128_t)nn * nn;   /* n^2 */
    __uint128_t n4 = n2 * n2;                 /* n^4 */
    __uint128_t lhs = (__uint128_t)120 * n4;  /* 120*n^4 */

    __uint128_t A   = (__uint128_t)s6 + s5 + 1;          /* s6 + s5 + 1 */
    __uint128_t A2  = A * A;                            /* A^2 */
    __int128_t  diffSigned = (__int128_t)A2 - (__int128_t)11 * n2;
    if (diffSigned < 0) diffSigned = -diffSigned;
    __uint128_t diff = (__uint128_t)diffSigned;
    __uint128_t rhs = diff * diff;                      /* (A^2 - 11*n^2)^2 */

    if (lhs >= rhs) base++; /* fractional parts sum to at least 1 */

    return (seq_t)base;
}
