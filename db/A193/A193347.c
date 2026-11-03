/*
 * A193347  Number of even divisors of tau(n).
 * https://oeis.org/A193347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of even divisors of τ(n) where τ(n) = number of
 *            divisors of n. For n ≥ 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 130 terms through n = 9528 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_divisors.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n=0, return 0 */

    /* τ(n) */
    uint64_t tau_n = num_divisors((uint64_t)n);
    if ((tau_n & 1ULL) == 1ULL) /* τ(n) is odd -> no even divisors */
        return 0;

    /* exponent of 2 in τ(n) */
    unsigned int r = 0;
    while ((tau_n & 1ULL) == 0ULL) {
        tau_n >>= 1;
        ++r;
    }

    /* τ(τ(n)/2^r) */
    uint64_t tau_m = num_divisors(tau_n);   /* tau_n now equals m */
    /* number of even divisors = r * τ(m) */
    return (seq_t)r * (seq_t)tau_m;
}
