/*
 * A193526  Sum of even divisors of sopf(n).
 * https://oeis.org/A193526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of even divisors of the sum of distinct primes
 *            dividing n (i.e. a(n) = σ(sopf(n)) – σ(sopf(n) / 2^k), where k ≥
 *            1 is the exponent of 2 in sopf(n); if sopf(n) is odd, a(n) = 0)
 * Range:     u8 n <= 410, u16 n <= 9348, u32 n <= 9348, u64 n <= 9348,
 *            u128 n <= 9348, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */

static seq_t sigma_of_uint64(uint64_t m)
{
    if (m == 1) return 1;          /* σ(1) = 1 */
    uint64_t p[64], e[64];
    size_t pcnt;
    factor_u64(m, p, e, &pcnt);

    seq_t result = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        seq_t term = 1;
        seq_t power = 1;
        for (uint64_t j = 1; j <= e[i]; ++j) {
            power *= (seq_t)p[i];
            term += power;
        }
        result *= term;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1) return 0;          /* a(1) = 0 */

    /* 1. Compute sopf(n) = sum of distinct primes dividing n. */
    uint64_t pp[64], ee[64];
    size_t pcnt;
    factor_u64((uint64_t)n, pp, ee, &pcnt);

    uint64_t sopf = 0;
    for (size_t i = 0; i < pcnt; ++i)
        sopf += pp[i];

    /* 2. If sopf is odd, there are no even divisors. */
    if ((sopf & 1ULL) == 0) {
        /* 3. Sigma of sopf. */
        seq_t sigma_sopf = sigma_of_uint64(sopf);

        /* 4. Remove all factors of 2 to obtain the odd part. */
        uint64_t odd_part = sopf;
        while ((odd_part & 1ULL) == 0) odd_part >>= 1;

        /* 5. Sigma of the odd part. */
        seq_t sigma_odd = sigma_of_uint64(odd_part);

        /* 6. Sum of even divisors = σ(sopf) – σ(odd_part). */
        return sigma_sopf - sigma_odd;
    }
    return 0;
}
