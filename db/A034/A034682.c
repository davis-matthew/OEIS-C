/*
 * A034682  Sum of eighth powers of unitary divisors.
 * https://oeis.org/A034682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^k‖n} (1 + p^(8*k))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 255,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* The following helper is assumed to be provided by the build system. */
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0) /* not part of offset, but keep defined */
        return 0;

    /* Arrays to hold prime factors and their exponents.
       Since n ≤ 2^64-1, at most 64 distinct prime factors. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        unsigned int k = (unsigned int)exps[i];
        idx_t exp = (idx_t)(8ULL * k);
        seq_t p_pow = pow_uint(primes[i], exp); /* p^(8*k) */
        seq_t term = p_pow + (seq_t)1;          /* 1 + p^(8*k) */
        result *= term;                         /* multiply into product */
    }
    return result;
}
