/*
 * A013937  a(n) = Sum_{k=1..n} floor(n/k^3).
 * https://oeis.org/A013937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(n/k^3)
 * Range:     u8 n <= 215, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9966 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>

/* integer cube root: largest k with k³ ≤ x */
static idx_t int_cube_root(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        __int128 cube = (__int128)mid * mid * mid;
        if (cube <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t limit = int_cube_root(n);          /* largest k with k^3 ≤ n */
    for (idx_t k = 1; k <= limit; ++k) {
        idx_t denom = k * k * k; /* k^3 fits in idx_t for all n up to ULLONG_MAX */
        sum += (seq_t)(n / denom); /* floor division in idx_t, cast to seq_t */
    }
    return sum;
}
