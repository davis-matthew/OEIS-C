/*
 * A034679  Sum of fifth powers of unitary divisors.
 * https://oeis.org/A034679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^k‖n} (1 + p^(5k))
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 84, u64 n <= 7085,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 46 terms through n = 9840 (2026-11-01)
 * Helpers:   factor_u64, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Unit: For n = ∏ p_j^k_j, a(n) = ∏ (1 + p_j^(5*k_j)) */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* Convert the index to a native 64‑bit integer for factoring. */
    uint64_t nn = (uint64_t)n;

    /* Arrays large enough for the number of distinct primes in a 64‑bit integer. */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt;

    /* Factor n into primes and exponents. */
    factor_u64(nn, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        /* Compute p^(5*k). */
        idx_t exp = 5 * (idx_t)exps[i];
        seq_t powv = ipow_idx((seq_t)primes[i], exp);
        /* Add 1 and multiply into the running product. */
        powv = powv + (seq_t)1;
        result = seq_mul(result, powv);
    }

    return result;
}
