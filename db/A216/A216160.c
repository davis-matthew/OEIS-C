/*
 * A216160  2^(2p-2) modulo p^3 for p=odd primes.
 * https://oeis.org/A216160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2p-2) (mod p^3), where p is the nth odd prime (a(1) =
 *            16 for p = 3). A prime sieve is regenerated only when more
 *            primes are required
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 267, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes.h"
#include "../utils/mod_pow.h"

static idx_t *prime_list = NULL;   /* array of primes up to prime_limit */
static size_t prime_cnt   = 0;      /* number of primes in prime_list */
static idx_t  prime_limit = 0;      /* maximum prime value currently stored */

/* rebuild the prime_list to contain all primes <= limit */
static void rebuild_primes(idx_t limit)
{
    if (prime_list) {
        free(prime_list);
        prime_list = NULL;
    }
    build_primes(limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

/* ensure we have at least the first (n+1) primes (including 2) */
static void ensure_primes(idx_t n)
{
    if (prime_cnt > n)        /* we already have enough */
        return;
    /* heuristic upper bound: 20 * (n+1) is safely larger than the nth odd prime
       for all n <= 10000 (prime_10001 ≈ 104743). */
    idx_t new_limit = (idx_t)(20 * (n + 1));
    if (new_limit <= prime_limit)
        new_limit = prime_limit + 1;   /* at least increase */
    rebuild_primes(new_limit);
}

seq_t a(idx_t n)
{
    if (n == 0)      /* offset is 1; undefined below */
        return 0;

    /* prime_list[0] = 2, prime_list[1] = 3, etc. */
    ensure_primes(n);                       /* need n+1 primes at least */
    seq_t p = (seq_t)prime_list[(size_t)n]; /* nth odd prime */

    seq_t mod = p * p * p;                  /* p^3 */

    /* exponent = 2p - 2 */
    idx_t exp = 2ULL * (idx_t)p - 2ULL;

    return mod_pow((seq_t)2, exp, mod);
}
