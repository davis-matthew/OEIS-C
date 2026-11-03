/*
 * A192066  Sum of the odd unitary divisors of n.
 * https://oeis.org/A192066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e||odd part of n} (p^e + 1)
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9834 (2026-11-01)
 * Helpers:   factor_u64, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Odd unitary divisors of n sum to the product of (p^e + 1) over all odd prime powers dividing n. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;            /* cast idx_t to 64‑bit integer */
    uint64_t primes[64], exps[64];
    size_t cnt;

    factor_u64(nn, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] == 2) continue;                /* skip prime 2 – its unitary divisor is only 1 */
        seq_t term = ipow_u64_idx(primes[i], exps[i]) + 1; /* p^e + 1 */
        result *= term;
    }
    return result;
}
