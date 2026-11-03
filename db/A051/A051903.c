/*
 * A051903  Maximum exponent in the prime factorization of n.
 * https://oeis.org/A051903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(1) = 0; for n > 1, a(n) = max exponent in the prime
 *            factorization of n
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 121 terms through n = 8893 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the factor_u64 utility to factor n and pick the largest exponent. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    /* Arrays large enough for any 64‑bit number (at most 64 distinct primes). */
    uint64_t pr[64];
    uint64_t exp[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, pr, exp, &cnt);

    seq_t maxexp = (seq_t)0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t e = (seq_t)exp[i];
        if (e > maxexp)
            maxexp = e;
    }

    return maxexp;
}
