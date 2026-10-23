/*
 * A129309  a(n) = number of primes which are < c(n) and are coprime to c(n),
 *          where c(n) is the n-th composite.
 * https://oeis.org/A129309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes < c(n) and coprime to c(n), where c(n)
 *            is the n-th composite number
 * Range:     u8 n <= 1369, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/prime_pi.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Find the n‑th composite number c(n). */
    idx_t cnt = 0;
    idx_t val = 4;  /* first composite */
    while (cnt < n) {
        if (!is_prime(val))
            ++cnt;
        if (cnt == n)
            break;
        ++val;
    }
    idx_t comp = val;  /* c(n) */

    /* Count primes p < comp. */
    seq_t primes_lt = prime_pi(comp - 1);

    /* Count distinct prime factors of comp. */
    uint64_t pfactors[64];
    uint64_t exps[64];
    size_t faccnt = 0;
    factor_u64((uint64_t)comp, pfactors, exps, &faccnt);

    /* Subtract the number of prime divisors from the prime count. */
    seq_t result = primes_lt - (seq_t)faccnt;
    return result;
}
