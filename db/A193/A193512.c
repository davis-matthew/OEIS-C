/*
 * A193512  a(n) = Sum of odd divisors of Omega(n), a(1) = 0.
 * https://oeis.org/A193512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, sum_odd_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A193512: a(n) = sum of odd divisors of Ω(n), Ω = A001222 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/sum_odd_divisors.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                     /* a(1) = 0 by convention */

    /* Factor n and sum exponents to get Ω(n). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    idx_t omega = 0;
    for (size_t i = 0; i < cnt; ++i) omega += exps[i];

    return sum_odd_divisors(omega);
}
