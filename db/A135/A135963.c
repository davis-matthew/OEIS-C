/*
 * A135963  a(n) = binomial(floor(n*(sqrt(5)+3)/2), n) for n>=0.
 * https://oeis.org/A135963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial( floor(n*(sqrt5+3)/2), n )
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 24 (2026-11-01)
 * Helpers:   binomial, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
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
    /* Compute floor(n*sqrt(5)) as floor_sqrt(5*n*n) */
    seq_t nsq   = (seq_t)n * (seq_t)n;
    seq_t prod  = (seq_t)5 * nsq;
    seq_t sumn  = floor_sqrt(prod);

    /* floor(n*phi) = floor((n + n*sqrt5)/2) */
    idx_t phi_floor = ((idx_t)n + (idx_t)sumn) / 2;

    /* floor(n*phi^2) = floor(n*phi) + n */
    idx_t phi2_floor = phi_floor + n;

    /* binomial(floor(n*phi^2), n) */
    return binomial(phi2_floor, n);
}
