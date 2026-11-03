/*
 * A034981  Smallest square starting with a string of n 2's.
 * https://oeis.org/A034981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest perfect square whose decimal representation
 *            begins with n consecutive '2's
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 13, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

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

seq_t a(idx_t n) {
    /* construct the prefix consisting of n twos:
       222...2  (n digits) = 2 * (10^n - 1) / 9 */
    seq_t pow10n  = pow10(n);                     /* 10^n */
    seq_t prefix  = (pow10n - 1) / 9 * 2;         /* 222...2 */

    seq_t pow10d = 1;                              /* 10^0 */
    for (;;) {
        seq_t lower = prefix * pow10d;                     /* prefix * 10^d   */
        seq_t upper = (prefix + 1) * pow10d - 1;           /* (prefix+1)*10^d-1 */

        seq_t s_low  = floor_sqrt(lower);                /* floor sqrt(lower) */
        if (s_low * s_low < lower) ++s_low;              /* ceil sqrt(lower) */

        seq_t s_high = floor_sqrt(upper);                /* floor sqrt(upper) */

        if (s_low <= s_high) {
            seq_t res = s_low * s_low;                  /* the square */
            return res;
        }

        /* try next order of magnitude */
        pow10d *= 10;
    }
}
