/*
 * A042315  Denominators of continued fraction convergents to sqrt(684).
 * https://oeis.org/A042315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the nth convergent to sqrt(684)
 * Range:     u8 none, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 199 (2026-11-01)
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

seq_t a(idx_t n)
{
    const seq_t D  = (seq_t)684;
    const seq_t a0 = floor_sqrt(D);

    seq_t m = 0;
    seq_t d = 1;
    seq_t q_pp = 1; /* q_{-2} */
    seq_t q_p  = 0; /* q_{-1} */
    seq_t q_c;

    /* k = 0 */
    q_c = a0 * q_p + q_pp;  /* = 1 */
    if (n == 0)
        return q_c;

    q_pp = q_p;
    q_p  = q_c;

    seq_t a = a0;
    for (idx_t k = 1; k <= n; ++k) {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        q_c = a * q_p + q_pp;
        if (k == n)
            return q_c;
        q_pp = q_p;
        q_p  = q_c;
    }

    return 0; /* never reached */
}
