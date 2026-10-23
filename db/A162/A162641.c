/*
 * A162641  Number of even exponents in canonical prime factorization of n.
 * https://oeis.org/A162641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of primes with even exponent in the prime
 *            factorization of n
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
    if (n <= 1) return 0;
    uint64_t num = (uint64_t)n;

    /* Temporary arrays for prime factors and exponents.
       For any 64‑bit integer the number of distinct prime factors is
       at most 16, so 20 is safely enough. */
    uint64_t pr[20], ex[20];
    size_t cnt = 0;

    factor_u64(num, pr, ex, &cnt);

    seq_t result = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if ((ex[i] & 1ULL) == 0ULL) ++result;  /* even exponent? */
    }
    return result;
}
