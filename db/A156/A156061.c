/*
 * A156061  a(n) = product of indices of distinct prime factors of n, where
 *          index(prime(k)) = k.
 * https://oeis.org/A156061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p|n, p prime} prime_pi(p) (product of the indices of
 *            the distinct prime factors of n)
 * Range:     u8 n <= 1618, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper declarations – the corresponding headers are supplied in the build system */
#include "../utils/factor_u64.h"    /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/prime_pi.h"     /* seq_t prime_pi(idx_t limit); */

seq_t a(idx_t n)
{
    /* a(1) is defined as 1 (empty product) */
    if (n == 1)
        return (seq_t)1;

    /* Factor n into distinct prime factors */
    uint64_t primes[64];
    uint64_t exps[64];   /* not used for the product, but required by factor_u64 */
    size_t   cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Product of the indices of the distinct primes */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t idx = prime_pi(primes[i]);   /* index (1‑based) of prime primes[i] */
        result = result * idx;
    }
    return result;
}
