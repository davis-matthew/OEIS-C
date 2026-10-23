/*
 * A142695  Primes congruent to 49 mod 57.
 * https://oeis.org/A142695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 49 (mod 57)
 * Range:     u8 n <= 1, u16 n <= 130, u32 n <= 156, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *prime_cache = NULL;   /* dynamically grown cache of found primes */
static size_t cache_cap   = 0;       /* current capacity of the cache        */
static size_t cache_len   = 0;       /* number of primes stored so far       */
static idx_t   k_next     = 0;       /* next k to try for candidate 57*k+49  */

/* Grow the cache so that it can hold at least 'needed' primes */
static void ensure_cache(idx_t needed)
{
    while (cache_len < (size_t)needed) {
        seq_t candidate = (seq_t)57 * (seq_t)k_next + (seq_t)49;
        if (is_prime_big(candidate)) {
            if (cache_len >= cache_cap) {
                size_t newcap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(prime_cache, newcap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);  /* OOM – should never happen in tests */
                prime_cache = tmp;
                cache_cap   = newcap;
            }
            prime_cache[cache_len++] = candidate;
        }
        ++k_next;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                  /* undefined for n<1, return 0 */
    ensure_cache(n);
    return prime_cache[n - 1];
}
