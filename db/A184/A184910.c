/*
 * A184910  a(n) = n + floor(n*r/s) + floor(n*t/s) with r = 2^(1/5), s = r^2,
 *          t = r^3.
 * https://oeis.org/A184910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n/r) + floor(n*r) where r = 2^(1/5)
 *            (≈1.148698354997): floor(n/r)=⌊n·2^(-1/5)⌋,
 *            floor(n*r)=⌊n·2^(1/5)⌋. These are computed exactly by fifth‑root
 *            tests: ⌊n·2^(1/5)⌋ = floor( (2·n⁵)^(1/5) ), ⌊n/r⌋ = floor(
 *            (n⁵/2)^(1/5) )
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                    /* defined only for n≥1 */

    /* Use 128‑bit arithmetic for the intermediate powers */
    __uint128_t N  = n;
    __uint128_t n5 = N * N * N * N * N;     /* n⁵ */

    /* ----- floor(n·r)  = floor( (2·n⁵)^(1/5) ) ----- */
    uint64_t low  = 0;
    uint64_t high = 2 * n;                   /* r ≈ 1.1487 < 2, so 2·n is safe */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        __uint128_t mid5 = (__uint128_t)mid * mid * mid * mid * mid;
        if (mid5 <= (n5 << 1))               /* mid⁵ ≤ 2·n⁵  */
            low = mid;
        else
            high = mid - 1;
    }
    uint64_t fr = low;          /* floor(n·r) */

    /* ----- floor(n/r)  = floor( (n⁵/2)^(1/5) ) ----- */
    low  = 0;
    high = n;                   /* 1/r < 1, so floor(n/r) < n */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        __uint128_t mid5 = (__uint128_t)mid * mid * mid * mid * mid;
        if (mid5 <= (n5 >> 1))               /* mid⁵ ≤ n⁵/2  */
            low = mid;
        else
            high = mid - 1;
    }
    uint64_t fld = low;        /* floor(n/r) */

    __uint128_t result = (__uint128_t)n + fr + fld;
    return (seq_t)result;
}
