/*
 * A167307  Totally multiplicative sequence with a(p) = 6*(p+2) for prime p.
 * https://oeis.org/A167307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (6*(p+2))^e (multiplicative, a(p)=6*(p+2))
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 127, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 59 terms through n = 928 (2026-11-01)
 * Helpers:   factor_u64, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
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
    /* a(1) = 1 by definition of multiplicative sequences. */
    if (n == 1)
        return (seq_t)1;

    /* Temporary buffers for prime factors and exponents.  A number < 2^64
       can have at most 64 distinct prime factors, so 64 is plenty. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t base = (seq_t)6 * (((seq_t)primes[i]) + (seq_t)2);
        seq_t pow  = ipow_idx(base, (idx_t)exps[i]); /* (6*(p+2))^e */
        result = result * pow;                      /* result *= pow */
    }
    return result;
}
