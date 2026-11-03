/*
 * A097850  a(n) = floor(2*sqrt(n)*phi(n)) - n.
 * https://oeis.org/A097850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(2*sqrt(n)*phi(n)) - n
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 1022, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9662 (2026-11-01)
 * Helpers:   ipow_i32, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/totient_seq.h"

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
    /* The sequence is defined for n >= 1. For the unlikely case
       n == 0 we simply return 0, but this is never used. */
    if (n == 0)
        return (seq_t)0;

    seq_t phi_n = totient_seq((seq_t)n);        /* φ(n) */
    seq_t phi_sq = phi_n * phi_n;            /* φ(n)^2 */
    seq_t m = (seq_t)4 * phi_sq * (seq_t)n;  /* 4·n·φ(n)^2 */
    seq_t s = floor_sqrt(m);                 /* floor( sqrt(m) ) */
    return s - (seq_t)n;                     /* a(n) */
}
