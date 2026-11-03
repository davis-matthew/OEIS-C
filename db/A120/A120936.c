/*
 * A120936  Number of primes in the interval [n,n+log(n)].
 * https://oeis.org/A120936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of primes p with n ≤ p ≤ n + ⌊log(n)⌋
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 121 terms through n = 4943 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <math.h>
#include "../utils/is_prime.h"

static inline idx_t floor_log_nat(idx_t n)
{
    /* compute floor(log(n)) using double precision; the value is < 45 for
       all 64‑bit integers, so the cast is exact. */
    return (idx_t)log((double)n);
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    idx_t k = floor_log_nat(n);
    idx_t upper = n + k;

    seq_t count = (seq_t)0;
    for (idx_t p = n; p <= upper; ++p)
        if (is_prime(p))
            ++count;

    return count;
}
