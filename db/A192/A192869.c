/*
 * A192869  Thin primes: odd primes p such that p+1 is a prime (or 1) times a
 *          power of two.
 * https://oeis.org/A192869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A192869: thin primes – odd primes p such that p+1 = q·2^k, where
   q is prime or 1.  The routine searches primes incrementally,
   keeps a cache of the found thin primes, and returns the n‑th term
   (n ≥ 1).  Calls may arrive in any order. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for n==0 */

    /* static cache of thin primes */
    static seq_t *cache = NULL;         /* array of found thin primes   */
    static size_t cache_cap = 0;        /* allocated capacity          */
    static size_t cache_len = 0;        /* number of thin primes stored */
    static idx_t last_checked = 1;      /* integer just before next test */

    /* grow the cache if we do not yet have n terms */
    if (cache_len < (size_t)n) {
        /* allocate or extend the cache */
        if (cache_cap == 0) {
            cache_cap = 1;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            if (!cache) return 0;       /* allocation failure */
        }
        while (cache_cap < (size_t)n) {
            size_t new_cap = cache_cap << 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;           /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* generate thin primes until we have n of them */
        while (cache_len < n) {
            /* next integer to test */
            last_checked++;

            /* skip even numbers – only 2 is even prime, which is not thin */
            if ((last_checked & 1ULL) == 0)
                continue;

            if (!is_prime(last_checked))
                continue;                 /* not a prime */

            /* compute odd part of p+1 */
            idx_t odd_part = last_checked + 1;
            while ((odd_part & 1ULL) == 0)
                odd_part >>= 1;

            /* thin prime condition: odd part is 1 or a prime */
            if (odd_part == 1 || is_prime(odd_part)) {
                cache[cache_len] = (seq_t)last_checked;
                cache_len++;
            }
        }
    }

    return cache[(size_t)n - 1];
}
