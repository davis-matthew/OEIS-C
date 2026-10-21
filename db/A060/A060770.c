/*
 * A060770  "Early" primes: upper ends of prime gaps that are smaller than the
 *          prime number theorem predicts.
 * https://oeis.org/A060770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A060770: early primes with p/log(p) - q/log(q) < 1,
   where q is the preceding prime.  The function builds a cache
   of qualifying primes so that calls with arbitrary n are fast. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "../utils/is_prime.h"

static seq_t *prime_cache = NULL;   /* stored qualifying primes     */
static size_t cache_len   = 0;       /* how many are stored          */
static size_t cache_cap   = 0;       /* allocated capacity           */
static idx_t last_prime   = 2;       /* last examined prime          */

/* Return the first prime >= start.  Uses is_prime(idx_t). */
static idx_t next_prime(idx_t start)
{
    if (start <= 2)
        return 2;
    if ((start & 1) == 0)
        ++start;                       /* make it odd              */
    for (idx_t cand = start; ; cand += 2)
    {
        if (is_prime(cand))
            return cand;
    }
}

/* Ensure that prime_cache contains at least n qualifying primes. */
static void ensure_cache(idx_t n)
{
    if (prime_cache == NULL)
    {
        cache_cap   = 128;
        prime_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_len < (size_t)n)
    {
        /* Advance to the next prime. */
        idx_t p = next_prime(last_prime + 1);
        idx_t q = last_prime;

        /* Compute p/log(p) - q/log(q) < 1 using double precision. */
        double diff = (double)p / log((double)p) - (double)q / log((double)q);

        if (diff < 1.0)
        {
            if (cache_len == cache_cap)
            {
                cache_cap *= 2;
                prime_cache = (seq_t *)realloc(prime_cache,
                                               cache_cap * sizeof(seq_t));
            }
            prime_cache[cache_len++] = (seq_t)p;
        }
        last_prime = p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined but safe      */
    ensure_cache(n);
    return prime_cache[n - 1];
}
