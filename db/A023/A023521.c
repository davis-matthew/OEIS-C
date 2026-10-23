/*
 * A023521  Sum of distinct prime divisors of prime(n)*prime(n-1) - 1.
 * https://oeis.org/A023521
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum of distinct prime divisors of prime(n)*prime(n-1) - 1
 *            Computed using get_prime and factor_u64 utilities
 * Range:     u8 n <= 10, u16 n <= 89, u32 n <= 8964, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/get_prime.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n < 2) /* undefined below offset; return 0 or any value */ return 0;

    uint32_t p_n   = get_prime(n);
    uint32_t p_nm1 = get_prime(n - 1);

    uint64_t prod = (uint64_t)p_n * (uint64_t)p_nm1;
    uint64_t m    = prod - 1;                /* prime(n)*prime(n-1) - 1 */

    /* Factor m. 64 distinct prime factors are enough for values up to ~1e10. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;

    factor_u64(m, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        sum += (seq_t)primes[i];
    }
    return sum;
}
