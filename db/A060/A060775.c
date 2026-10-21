/*
 * A060775  The greatest divisor d|n such that d < n/d, with a(1) = 1.
 * https://oeis.org/A060775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = max{ d : d|n and d < sqrt(n) } for n > 1, a(1) = 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute floor(sqrt(n)) for an unsigned 64‑bit integer.
   The algorithm uses only integer arithmetic and avoids overflow. */
static idx_t isqrt_idx(idx_t n)
{
    idx_t lo = 0, hi = (n >> 1) + 1;    /* hi is always >= floor(sqrt(n)) */
    while (lo < hi) {
        /* Midpoint biased upwards; ensure mid <= hi */
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid > 0 && mid > n / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;          /* a(1) = 1 by definition */

    idx_t dmax = isqrt_idx(n);    /* largest integer ≤ sqrt(n) */

    for (idx_t d = dmax; d > 0; --d) {
        if (n % d == 0) {          /* d is a divisor of n */
            if (d < n / d)        /* strict inequality d < n/d */
                return (seq_t)d;  /* first (i.e., largest) meeting the condition */
        }
    }

    /* For n > 1 this point is unreachable because d = 1 always satisfies
       the conditions.  The return below is only to keep the compiler
       happy. */
    return (seq_t)1;
}
