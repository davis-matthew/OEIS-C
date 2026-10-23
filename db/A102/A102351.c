/*
 * A102351  Let p = prime(n); then a(n) = number of residues p mod q which are
 *          prime, as q runs through the primes less than p.
 * https://oeis.org/A102351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes q < prime(n) such that prime(n) % q is
 *            prime
 * Range:     u8 n <= 54, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   get_prime, is_prime, simple_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/simple_sieve.h"
#include "../utils/get_prime.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1 */

    /* Get the n-th prime */
    uint64_t p_val = (uint64_t)get_prime(n);
    uint32_t p = (uint32_t)p_val;  /* fits in 32 bits for n <= 10000 */

    /* Sieve all primes up to p (including p) */
    uint32_t *primes = NULL;
    size_t cnt = 0;
    simple_sieve(p, &primes, &cnt);

    /* Count prime residues */
    seq_t count = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint32_t q = primes[i];
        if (q >= p) break;
        uint64_t r = p % q;
        if (is_prime(r)) {
            count++;
        }
    }

    free(primes);
    return count;
}
