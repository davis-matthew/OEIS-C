/*
 * A020740  Max_{k=0..n} d(C(n,k)) - d(C(n,[ n/2 ])), where d() = number of
 *          divisors.
 * https://oeis.org/A020740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{k=0..n} d(C(n,k)) - d(C(n,floor(n/2)))
 * Range:     u8 n <= 26, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* Legendre's formula: exponent of prime p in m! */
static inline uint32_t exp_in_fact(idx_t p, idx_t m)
{
    uint32_t e = 0;
    while (m) {
        m /= p;
        e += (uint32_t)m;
    }
    return e;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t mid = n / 2;

    /* Build list of primes up to n */
    uint64_t *primes;
    size_t prime_cnt;
    build_primes_upto(n, &primes, &prime_cnt);

    /* Exponent of each prime in n! */
    uint32_t *exp_n = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    for (size_t i = 0; i < prime_cnt; ++i)
        exp_n[i] = exp_in_fact(primes[i], n);

    seq_t max_d = 0;
    seq_t mid_d = 0;

    for (idx_t k = 0; k <= mid; ++k) {
        seq_t prod = 1;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t e = exp_n[i];
            e -= exp_in_fact(primes[i], k);
            e -= exp_in_fact(primes[i], n - k);
            prod *= (seq_t)(e + 1);
        }
        if (k == mid)
            mid_d = prod;
        if (prod > max_d)
            max_d = prod;
    }

    seq_t result = max_d - mid_d;

    free(exp_n);
    free(primes);
    return result;
}
