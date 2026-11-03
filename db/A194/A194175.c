/*
 * A194175  [sum{(k*r) : 1<=k<=n}], where [ ]=floor, ( )=fractional part, and
 *          r=sqrt(7).
 * https://oeis.org/A194175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( sqrt(7) * n*(n+1)/2 ) – Σ_{k=1}^{n}
 *            floor(k*sqrt(7))
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           = floor( sqrt(7) * n*(n+1)/2 ) – Σ_{k=1}^{n} floor( sqrt(7*k*k) ) */

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

/* Return the n-th term of OEIS A194175. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* triangular number T = n*(n+1)/2, computed using idx_t first. */
    idx_t T = n * (n + 1) / 2;

    /* floor( sqrt(7) * T )  = floor( sqrt(7*T^2) ) */
    seq_t top_val = (seq_t)7 * (seq_t)T * (seq_t)T;
    seq_t top = floor_sqrt(top_val);

    /* Σ floor( sqrt(7*k^2) ) for k = 1..n */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t v = (seq_t)7 * (seq_t)k * (seq_t)k;
        sum += floor_sqrt(v);
    }

    return top - sum;
}
