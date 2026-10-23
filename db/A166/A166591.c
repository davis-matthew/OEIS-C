/*
 * A166591  Totally multiplicative sequence with a(p) = p+3 for prime p.
 * https://oeis.org/A166591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over prime powers p^e | n of (p+3)^e
 * Range:     u8 n <= 15, u16 n <= 127, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is totally multiplicative with a(p) = p + 3 for primes p. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_one.h"

seq_t a(idx_t n) {
    if (n == 0) return seq_one(); /* offset handling: a(1) for n=1 */

    /* Small temporary arrays for prime factors; 64 elements is more than enough for n up to 2^64. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* Factor n into primes and exponents */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = seq_one(); /* multiplicative identity */

    for (size_t i = 0; i < cnt; ++i) {
        /* base = p + 3, converted to seq_t */
        seq_t base = (seq_t)(primes[i] + 3);
        seq_t term  = seq_one();

        /* compute (p+3)^e by repeated multiplication */
        for (uint64_t e = 0; e < exps[i]; ++e) {
            term *= base;
        }

        /* multiply into the running product */
        result *= term;
    }

    return result;
}
