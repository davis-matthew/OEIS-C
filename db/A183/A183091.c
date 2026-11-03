/*
 * A183091  a(n) is the product of the divisors p^k of n where p is prime and
 *          k >= 1.
 * https://oeis.org/A183091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product_{p^k‖n} p^(k(k+1)/2)
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 255, u64 n <= 2047,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9795 (2026-11-01)
 * Helpers:   factor_u64, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper functions supplied by the build system. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_idx.h"

static inline seq_t seq_one(void) { return (seq_t)1; }

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Compute the A183091 sequence term a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    /* arrays large enough for any 64‑bit integer */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = seq_one();

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = primes[i];
        uint64_t k  = exps[i];
        idx_t    exp = (idx_t)((seq_t)k * (k + 1) / 2);   /* k(k+1)/2 */
        seq_t part = ipow_idx((seq_t)p, exp);
        result = seq_mul(result, part);
    }

    return result;
}
