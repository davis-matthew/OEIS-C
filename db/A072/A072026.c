/*
 * A072026  Swap twin prime pairs >(3,5) in prime factorization of n.
 * https://oeis.org/A072026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = \prod_{p^e||n} f(p)^e where f(p) = p if p<=3 p+2 if p+2
 *            is prime p-2 if p-2 is prime p otherwise This implements the
 *            OEIS sequence A072026
 * Range:     u8 n <= 124, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 1. */
    if (n == 1) return (seq_t)1;

    /* Factor n into primes and corresponding exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t mapped;

        if (p <= 3) {
            mapped = p;
        } else if (is_prime_small((seq_t)(p + 2))) {
            mapped = p + 2;
        } else if (is_prime_small((seq_t)(p - 2))) {
            mapped = p - 2;
        } else {
            mapped = p;
        }

        /* Multiply result by mapped^e. */
        for (uint64_t k = 0; k < e; ++k) {
            result *= (seq_t)mapped;
        }
    }

    return result;
}
