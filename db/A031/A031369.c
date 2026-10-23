/*
 * A031369  a(n) = prime(3n-1).
 * https://oeis.org/A031369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(3 * n - 1) (first prime at position 3n‑1)
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS sequence A031369.
 *
 * The function keeps a growing array of primes in static memory.
 * For each request it ensures that at least 3n-1 primes are stored
 * and returns the prime at the (3n-1)‑th position.
 *
 * This implementation uses trial division by already known primes.
 * It is fast enough for n up to about 10 000 (the required range).  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static cache of primes.  */
static idx_t  *prime_cache = NULL;
static size_t  prime_cnt   = 0;   /* number of primes stored       */
static size_t  prime_cap   = 0;   /* current capacity of array    */

/* Ensure that the cache contains at least `needed` primes. */
static void
ensure_prime_count(size_t needed)
{
    if (prime_cnt >= needed)
        return;

    /* Initialise cache if first call. */
    if (prime_cap == 0) {
        prime_cap   = 512;                   /* initial size */
        prime_cache = (idx_t *)malloc(sizeof(idx_t) * prime_cap);
        prime_cache[0] = 2;                  /* first prime   */
        prime_cnt   = 1;
    }

    /* Generate primes until we reach the required count. */
    while (prime_cnt < needed)
    {
        idx_t candidate = prime_cache[prime_cnt - 1] + 1;

        /* Find the next prime by trial division. */
        for (;; ++candidate)
        {
            bool is_prime = true;

            for (size_t i = 0; i < prime_cnt; ++i)
            {
                idx_t p = prime_cache[i];
                if ((idx_t)p * (idx_t)p > candidate)   /* reached sqrt */
                    break;
                if (candidate % p == 0) {
                    is_prime = false;
                    break;
                }
            }

            if (is_prime) {
                /* Grow the array if necessary. */
                if (prime_cnt == prime_cap) {
                    prime_cap *= 2;
                    idx_t *tmp = (idx_t *)realloc(prime_cache, sizeof(idx_t) * prime_cap);
                    if (!tmp) /* allocation failure – terminate. */
                        exit(1);
                    prime_cache = tmp;
                }
                prime_cache[prime_cnt++] = candidate;
                break;
            }
        }
    }
}

/* Public sequence function. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for n=0, safe fallback */

    /* Desired prime index (1‑based). */
    idx_t k = 3 * n - 1;

    ensure_prime_count((size_t)k);

    /* primes_cache is 0‑based; k is 1‑based. */
    return (seq_t)prime_cache[k - 1];
}
