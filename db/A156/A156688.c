/*
 * A156688  The total number of distinct Pythagorean triples with an area
 *          numerically equal to n times their perimeters.
 * https://oeis.org/A156688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = (2 + e₂) * Π_{p>2} (2·e_p + 1) where e₂ and e_p are the
 *            exponents of prime 2 and other primes in the prime factorization
 *            of n. This is equivalent to a(n) = d(8·n²)/2, the half of the
 *            divisor count of 8·n²
 * Range:     u8 n <= 3464, u16 n <= 8826, u32 n <= 8826, u64 n <= 8826,
 *            u128 n <= 8826, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* factorize n */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    uint64_t e2 = 0;

    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] == 2) {
            e2 = exps[i];
        } else {
            /* multiply by (2*e + 1) for each odd prime factor */
            seq_t part = (seq_t)(2 * exps[i] + 1);
            result *= part;
        }
    }

    /* multiply by (2 + e₂) */
    seq_t factor2 = (seq_t)(e2 + 2);
    result *= factor2;

    return result;
}
