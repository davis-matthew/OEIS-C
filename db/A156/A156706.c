/*
 * A156706  For all numbers k(n) congruent to +1 or -1 (mod 6) starting with
 *          k(n) = {5,7,11,13,...}, a(k(n)) is the congruence (mod 6) if k(n)
 *          is prime and 0 if k(n) is composite.
 * https://oeis.org/A156706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (n even ? 1 : -1) if k(n) is prime; 0 otherwise, where
 *            k(n) = 6*ceil(n/2)+(-1)^n
 * Range:     i8 n <= 9119, i16 n <= 9119, i32 n <= 9119, i64 n <= 9119,
 *            i128 n <= 9119, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Prime sieve cache */
static uint8_t *prime_cache = NULL;
static idx_t prime_cache_limit = 0;

/* Rebuild the prime cache up to 'limit' (inclusive). */
static void sieve_primes_upto(idx_t limit)
{
    uint8_t *new_cache = (uint8_t *)realloc(prime_cache, (limit + 1) * sizeof(uint8_t));
    if (!new_cache) return;               /* allocation failure is ignored */

    new_cache[0] = 0;
    if (limit >= 1) new_cache[1] = 0;
    for (idx_t i = 2; i <= limit; ++i) new_cache[i] = 1;

    for (idx_t p = 2; p * p <= limit; ++p)
    {
        if (new_cache[p])
        {
            for (idx_t m = p * p; m <= limit; m += p)
                new_cache[m] = 0;
        }
    }

    prime_cache = new_cache;
    prime_cache_limit = limit;
}

/* Return true iff n is prime, extending the sieve if necessary. */
static bool is_prime(idx_t n)
{
    if (n <= prime_cache_limit) return prime_cache[n] != 0;
    /* number larger than cached limit – rebuild the sieve up to it */
    sieve_primes_upto(n);
    return prime_cache[n] != 0;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* compute k(n) = 6*ceil(n/2)+(-1)^n */
    idx_t half = (n + 1) / 2;                /* ceil(n/2) */
    idx_t k = (n & 1) ? (6 * half - 1) : (6 * half + 1);

    if (!is_prime(k)) return 0;
    return (n & 1) ? (seq_t)-1 : (seq_t)1;
}
