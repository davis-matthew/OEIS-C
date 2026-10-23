/*
 * A142844  Primes congruent to 46 mod 61.
 * https://oeis.org/A142844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 108, u32 n <= 83, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the primes congruent to 46 modulo 61.
   a(n) = the n‑th prime p such that p ≡ 46 (mod 61).  */

/* OEIS link: A142844  Primes congruent to 46 mod 61 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

/* Cache for previously computed terms (zero‑based).  */
static seq_t *cache = NULL;
static size_t cache_count = 0;   /* number of terms stored in cache */
static size_t cache_cap   = 0;   /* allocated capacity of cache */
static idx_t  last_k      = 0;   /* last k examined (k such that p = 46 + 61*k) */

/* Return the n-th term, where the sequence is 1‑based.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for offset 0 */

    /* Return a cached value if it already exists. */
    if (cache != NULL && (size_t)n <= cache_count)
        return cache[n - 1];

    /* Grow the cache if it doesn't exist or is too small.  */
    if (cache == NULL) {
        cache_cap   = (size_t)(n > 128 ? n : 128);
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_count = 0;
        last_k      = 0;
    } else if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n)
            new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            abort();   /* out of memory */
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Generate missing terms.  */
    while (cache_count < (size_t)n) {
        ++last_k;                                   /* next candidate k */

        /* Compute p = 46 + 61*k in a type that can hold it.  */
        uint64_t p64 = 46ULL + 61ULL * (uint64_t)last_k;

        /* Cast to seq_t for the primality test.  If seq_t is too small
           to hold p64, the cast will overflow and p will be wrong,
           but such a value will not be needed to return a valid term
           that fits into seq_t.  */
        seq_t p = (seq_t)p64;

        if (is_prime_big(p))
            cache[cache_count++] = p;
    }

    return cache[n - 1];
}
