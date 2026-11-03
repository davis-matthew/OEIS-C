/*
 * A184912  n+[ns/r]+[nt/r]+[nu/r], where []=floor and r=2^(1/5), s=r^2,
 *          t=r^3, u=r^4.
 * https://oeis.org/A184912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*2^(1/5)) + floor(n*2^(2/5)) +
 *            floor(n*2^(3/5))
 * Range:     u8 n <= 51, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Validated: partial, pass, 120 terms through n = 120 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>

/* Compute floor of x^(1/5) given x as unsigned __int128. */
static uint64_t fifth_root_floor_uint128(unsigned __int128 N) {
    unsigned __int128 low = 0, high = 1;
    /* Find an upper bound */
    while (high * high * high * high * high <= N)
        high <<= 1;
    low = high >> 1;
    /* Binary search */
    while (low + 1 < high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 val = mid * mid * mid * mid * mid;
        if (val <= N)
            low = mid;
        else
            high = mid;
    }
    return (uint64_t)low;
}

/* Compute floor(n * 2^(k/5)) for k=1,2,3 using integer 5th root
 * via (n^5 * 2^k)^(1/5) */
static uint64_t floor_scale(idx_t n, unsigned k) {
    unsigned __int128 rn = (unsigned __int128)n;
    /* n^5 */
    unsigned __int128 n5 = rn * rn * rn * rn * rn;
    /* Multiply by 2^k (shift left by k bits) */
    unsigned __int128 N = n5 << k; /* 2^k */
    return fifth_root_floor_uint128(N);
}

seq_t a(idx_t n) {
    /* a is defined for n >= 1 */
    if (n == 0)
        return (seq_t)0; /* Not part of the sequence but harmless */

    uint64_t rn = (uint64_t)n;
    uint64_t r1 = floor_scale(rn, 1); /* floor(n * 2^(1/5))   */
    uint64_t r2 = floor_scale(rn, 2); /* floor(n * 2^(2/5))   */
    uint64_t r3 = floor_scale(rn, 3); /* floor(n * 2^(3/5))   */

    seq_t result = (seq_t)(rn + r1 + r2 + r3);
    return result;
}
