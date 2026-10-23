/*
 * A046951  a(n) is the number of squares dividing n.
 * https://oeis.org/A046951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn, mult, easy
 * Formula:   a(n) = product_{p^e||n} (⌊e/2⌋ + 1) (number of square divisors
 *            of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* n ≥ 1. For n = 1 the product is empty → 1. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    if (n == 1)
        return (seq_t)1;

    /* factor_u64 supplies the prime factorisation of n. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        /* floor(e/2) + 1 for each prime power p^e */
        seq_t factor = (seq_t)(exps[i] / 2 + 1);
        result *= factor;
    }
    return result;
}
