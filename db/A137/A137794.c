/*
 * A137794  Characteristic function of numbers having no prime gaps in their
 *          factorization.
 * https://oeis.org/A137794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the distinct prime factors of n form a consecutive
 *            run of primes; otherwise a(n) = 0
 * Range:     u8 n <= 9605, u16 n <= 9605, u32 n <= 9605, u64 n <= 9605,
 *            u128 n <= 9605, big any n
 * Helpers:   build_primes_upto, int_sqrt, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Utility functions from the build */
#include "../utils/build_primes_upto.h"
#include "../utils/next_prime.h"
#include "../utils/int_sqrt.h"   /* not used directly but available */

/* static cache of primes */
static uint64_t *prime_list   = NULL;
static size_t   prime_count   = 0;
static idx_t    prime_limit   = 0;

static void
ensure_prime_cache_for(idx_t limit)
{
    if (limit <= prime_limit)
        return;
    uint64_t *list = NULL;
    size_t   cnt   = 0;
    build_primes_upto(limit, &list, &cnt);
    if (prime_list)
        free(prime_list);
    prime_list  = list;
    prime_count = cnt;
    prime_limit = limit;
}

seq_t a(idx_t n)
{
    /* For the offset, return 1 for n == 1 (and also for n == 0 if
       the function is called outside the documented range). */
    if (n <= 1)
        return (seq_t)1;

    /* We need primes up to n to get next_prime values for the largest
       prime factor.  Rebuild the cache only if the required limit grows. */
    ensure_prime_cache_for(n);

    /* Factor n, collecting distinct prime factors in ascending order. */
    idx_t tmp   = n;
    uint64_t facs[64];
    int      cnt  = 0;

    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint64_t p = prime_list[i];
        if (tmp % p == 0) {
            facs[cnt++] = p;          /* store distinct prime once */
            while (tmp % p == 0)
                tmp /= p;
        }
    }
    if (tmp > 1) {                     /* remaining large prime factor */
        facs[cnt++] = tmp;
    }

    /* Every sequence of one prime factor is trivially consecutive. */
    if (cnt <= 1)
        return (seq_t)1;

    /* Check that the primes are consecutive. */
    for (int i = 1; i < cnt; ++i) {
        /* next_prime expects an idx_t (uint64_t), which is the type of facs[i-1] */
        if (next_prime(facs[i - 1]) != facs[i])
            return (seq_t)0;
    }

    return (seq_t)1;
}
