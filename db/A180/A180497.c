/*
 * A180497  a(n)=n^2-3*floor[n/sqrt(3)]^2.
 * https://oeis.org/A180497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2-3*floor[n/sqrt(3)]^2
 * Range:     u8 n <= 13, u16 n <= 169, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 85 terms through n = 972 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
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
#include <stdbool.h>

/* Compute a(n) as defined in OEIS A180497 */
seq_t a(idx_t n)
{
    /* According to the offset, n >= 1.  For n < 1 we simply return 0. */
    if (n == 0) return 0;

    /* Work entirely in integer arithmetic. */
    seq_t sq   = (seq_t)n * (seq_t)n;            /* n^2  */
    seq_t div  = sq / 3;                         /* floor(n^2 / 3)  */
    seq_t k    = floor_sqrt(div);                /* k = floor(n/√3) */
    seq_t res  = sq - 3 * k * k;                 /* a(n) = n^2 - 3k^2   */
    return res;
}
