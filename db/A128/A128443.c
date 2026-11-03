/*
 * A128443  a(n) = floor(n*3^(n/2)).
 * https://oeis.org/A128443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*3^(n/2))
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 32 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Compute 3^exp as a seq_t, using exponentiation by squaring. */
static seq_t pow3(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 3;
    while (exp > 0) {
        if (exp & 1ULL) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safety for indices < 1, not used */

    if (n & 1ULL) {                /* odd n: a(n) = floor(n*3^((n-1)/2)*sqrt(3)) */
        idx_t exp = (n - 1) >> 1;  /* (n-1)/2 */
        seq_t pow3e = pow3(exp);
        seq_t m     = (seq_t)n * pow3e;   /* m = n * 3^((n‑1)/2) */
        seq_t t = m * m;                 /* m^2 */
        t = t * 3;                       /* m^2*3 */
        return floor_sqrt(t);            /* floor(sqrt(m^2*3)) = floor(m*sqrt(3)) */
    } else {                       /* even n: a(n) = n*3^(n/2) */
        idx_t exp = n >> 1;          /* n/2 */
        seq_t pow3e = pow3(exp);
        return (seq_t)n * pow3e;
    }
}
