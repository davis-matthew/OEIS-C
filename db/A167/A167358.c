/*
 * A167358  Totally multiplicative sequence with a(p) = (p+2)^2 = p^2+4p+4 for
 *          prime p.
 * https://oeis.org/A167358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Product over primes p^e in n of (p+2)^(2e)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. a(n) = ∏_p (p+2)^(2·v_p(n)) */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* factor_u64: void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/big_pow.h"      /* big_pow: seq_t big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    if (n == 1u)                /* a(1) = 1 */
        return (seq_t)1;

    uint64_t n64 = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t   cnt    = 0;

    factor_u64(n64, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t base   = (seq_t)(primes[i] + 2);
        seq_t exp    = (seq_t)(2 * exps[i]);      /* 2e */
        seq_t term   = big_pow(base, exp);
        result *= term;
    }

    return result;
}
