/*
 * A069115  Squarefree part of prime(n)+1 : the smallest number such that
 *          a(n)*(prime(n)+1) is a square.
 * https://oeis.org/A069115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = squarefree part of prime(n)+1 (the smallest integer a
 *            such that a*(prime(n)+1) is a square)
 * Range:     u8 n <= 18, u16 n <= 799, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, floor_sqrt, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Helpers from the utils library */
#include "../utils/get_prime.h"          /* uint32_t get_prime(idx_t k); */
#include "../utils/floor_sqrt.h"         /* seq_t floor_sqrt(seq_t x); */
#include "../utils/build_primes_upto.h"  /* void build_primes_upto(idx_t limit, uint64_t **out, size_t *cnt); */

extern void free(void *ptr);     /* C standard free */

/* Cached list of primes used for factorisation */
static uint64_t *prime_cache = NULL;
static size_t prime_cache_cnt = 0;
static idx_t  prime_cache_limit = 0;

/* Return the squarefree part of an integer: product of primes with odd exponent */
static seq_t squarefree_part(seq_t m)
{
    /* quick path: if m is 0 or 1 */
    if (m <= 1)
        return m;

    /* compute integer square root of m to bound the factors */
    seq_t sqrt_m = floor_sqrt(m);
    idx_t bound = (idx_t)sqrt_m;
    if (bound < 2) bound = 2;

    /* resize prime cache if necessary */
    if (bound > prime_cache_limit || prime_cache == NULL) {
        if (prime_cache != NULL) {
            free(prime_cache);
            prime_cache = NULL;
            prime_cache_cnt = 0;
        }
        build_primes_upto(bound, &prime_cache, &prime_cache_cnt);
        prime_cache_limit = bound;
    }

    seq_t result = 1;
    /* trial division using cached primes */
    for (size_t i = 0; i < prime_cache_cnt; ++i) {
        seq_t p = (seq_t)prime_cache[i];
        if (p * p > m) break;

        unsigned int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        if (exp & 1)          /* odd exponent contributes to squarefree part */
            result *= p;
    }

    /* If m > 1 now, it is a prime factor with exponent 1 */
    if (m > 1)
        result *= m;

    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;             /* undefined for n=0; make it harmless */

    /* Get the nth prime (1‑based) */
    uint32_t p = get_prime(n);

    /* Compute prime(n)+1 as seq_t */
    seq_t m = (seq_t)p + 1;

    /* Return its squarefree part */
    return squarefree_part(m);
}
