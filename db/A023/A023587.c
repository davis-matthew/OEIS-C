/*
 * A023587  a(n) = sum of distinct prime divisors of 2*prime(n)-1.
 * https://oeis.org/A023587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = sum of distinct prime divisors of 2*prime(n)-1
 * Range:     u8 n <= 33, u16 n <= 3513, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    /* get the n-th prime (OEIS offset 1) */
    uint32_t p = get_prime(n);
    /* compute 2*p - 1 */
    uint64_t m = (uint64_t)2 * p - 1;

    /* factor m into distinct primes */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    /* sum the distinct prime factors */
    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        sum += (seq_t)primes[i];
    }
    return sum;
}
