/*
 * A219966  Number of ways to write n=p+q+(n mod 2)q with q<=n/2 and p, q, q+6
 *          all prime.
 * https://oeis.org/A219966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A219966
 *  Count of representations n = p + q + (n mod 2) q
 *  with primes p, q, q+6 and q ≤ n/2.
 *  Formula: a(n) = #{ (p,q) | p and q are prime,
 *               q ≤ n/2, q+6 is prime,
 *               p = n - (1 + (n mod 2)) q }   (n ≥ 1)
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"

/* Cache of primes up to the maximum n seen so far. */
static idx_t prime_limit = 0;
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset starts at 1 */

    /* Rebuild prime list if a larger n is requested. */
    if (n > prime_limit) {
        free(prime_list);
        build_primes_upto(n, &prime_list, &prime_count);
        prime_limit = n;
    }

    seq_t count = 0;
    idx_t mod2   = n & (idx_t)1;          /* n mod 2 (0 or 1) */
    idx_t k      = (idx_t)1 + mod2;       /* multiplier for q in p = n - k*q */
    idx_t half   = n >> 1;                /* floor(n/2) */

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t q = prime_list[i];
        if (q > half) break;                /* q must not exceed n/2 */

        if (!is_prime((idx_t)(q + 6))) continue;  /* q+6 has to be prime */

        idx_t p = n - k * q;                /* p = n - (1+mod2)*q */
        if (p < 2) continue;                /* p must be at least 2 */

        if (is_prime(p)) ++count;           /* q and p are prime */
    }

    return count;
}
