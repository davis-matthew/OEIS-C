/*
 * A189463  a(n) = [3*n*r] - 3*[n*r], where r=sqrt(5).
 * https://oeis.org/A189463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(3*n*sqrt(5)) - 3*floor(n*sqrt(5))
 * Range:     u8 n <= 1, u16 n <= 19, u32 n <= 4655, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 134 terms through n = 9227 (2026-11-01)
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
    /* The sequence is defined for n >= 1.  For n == 0 we return 0
       only to keep the function well‑defined; the offset in OEIS
       starts at 1. */
    if (n == 0) return 0;

    seq_t nn = (seq_t)n;           /* cast index to the integer type used for the values   */
    seq_t nsq = nn * nn;           /* n², all operations done in seq_t                      */
    seq_t prod5 = 5 * nsq;         /* 5 · n²                                                   */
    seq_t prod45 = 45 * nsq;       /* 45 · n² = (3n)² · 5                                      */

    seq_t r1 = floor_sqrt(prod5);   /* floor(n·√5)    */
    seq_t r2 = floor_sqrt(prod45);  /* floor(3n·√5)  */

    return r2 - 3 * r1;
}
