/*
 * A193512  a(n) = Sum of odd divisors of Omega(n), a(1) = 0.
 * https://oeis.org/A193512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9717 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A193512: a(n) = sum of odd divisors of Ω(n), Ω = A001222 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Sum of odd divisors of an unsigned integer k (k fits in idx_t). */
static seq_t sum_odd_divisors(idx_t k)
{
    idx_t sum = 0;
    for (idx_t d = 1; d * d <= k; ++d) {
        if (k % d == 0) {
            if (d & 1) sum += d;
            idx_t e = k / d;
            if (e != d && (e & 1)) sum += e;
        }
    }
    return (seq_t)sum;
}

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
