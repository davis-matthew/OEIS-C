/*
 * A077503  Smallest n-digit square beginning with n.
 * https://oeis.org/A077503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = ceiling(sqrt(n*10^d))^2 where d = n - digit(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-01)
 * Helpers:   ipow_i32, num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/ipow_i32.h"
#include "../utils/num_digits0.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

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

seq_t a(idx_t n)
{
    /* n ≥ 1 in the intended domain */
    idx_t d = n - num_digits0(n);          /* digits beyond the leading pattern */
    seq_t mult = (seq_t)n;                 /* cast index to seq_t for multiplication */
    seq_t powv = pow10(d);                 /* 10^d as a seq_t value */
    seq_t m = mult * powv;                 /* n * 10^d */

    seq_t s = floor_sqrt(m);               /* floor(sqrt(m)) */
    /* If not a perfect square, increase to the next integer */
    if (s * s < m)
        s += 1;

    return s * s;                          /* square the adjusted root */
}
