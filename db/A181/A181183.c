/*
 * A181183  a(n) = sigma(tau(n)) (mod 2).
 * https://oeis.org/A181183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(tau(n)) (mod 2)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9485 (2026-11-01)
 * Helpers:   factor_u64, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   sigma(tau(n)) is odd iff tau(n) is a square or twice a square.      */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
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

seq_t a(idx_t n) {
    /* Compute the divisor count τ(n). */
    uint64_t nn = (uint64_t)n;
    size_t cnt = 0;
    uint64_t primes[64];
    uint64_t exps[64];
    factor_u64(nn, primes, exps, &cnt);

    seq_t tau = 1;
    for (size_t i = 0; i < cnt; ++i)
        tau *= (seq_t)(exps[i] + 1);

    /* Determine parity of sigma(tau).  sigma(m) is odd ⇔ m is a square or twice a square. */
    seq_t r = floor_sqrt(tau);
    if (r * r == tau)
        return (seq_t)1;

    seq_t half = tau / 2;
    seq_t s = floor_sqrt(half);
    if (s * s * 2 == tau)
        return (seq_t)1;

    return (seq_t)0;
}
