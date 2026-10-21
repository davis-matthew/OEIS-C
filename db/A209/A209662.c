/*
 * A209662  a(n) = (-1)^A083025(n)*n.
 * https://oeis.org/A209662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac, easy, mult
 * Formula:   a(n) = (-1)^A083025(n) * n A083025(n) is the number of prime
 *            factors ≡ 1 (mod 4) counted with multiplicity
 * Range:     i8 n <= 127, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Compute a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0)  /* offset starts at 1, but guard against accidental 0 */
        return 0;

    /* Temporary arrays for prime factors and their exponents.
       64‑bit numbers have at most 15 distinct prime factors, so 20 is safe. */
    static uint64_t primes[20], exps[20];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Count the sum of exponents of primes ≡ 1 (mod 4) modulo 2. */
    unsigned parity = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if ((primes[i] & 3ULL) == 1ULL)   /* primes[i] mod 4 == 1 */
            parity ^= (unsigned)(exps[i] & 1ULL);
    }

    /* Sign is -1 if parity is odd, else +1. */
    seq_t sign = (parity ? (seq_t)-1 : (seq_t)1);
    return (seq_t)n * sign;
}
