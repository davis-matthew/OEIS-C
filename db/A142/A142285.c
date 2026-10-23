/*
 * A142285  Primes congruent to 36 mod 43.
 * https://oeis.org/A142285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime that is ≡ 36 (mod 43)
 * Range:     u8 n <= 2, u16 n <= 158, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/*
 * Return the nth prime congruent to 36 modulo 43.
 * n ≥ 1.  For n == 0 the function returns 0 as a safe fallback.
 *
 * The function keeps a static cache of found primes so that calls
 * in arbitrary order are fast.  The cache is resized with realloc
 * and a cast is added to allow compilation under C++ where the cast
 * from void* to seq_t* is not implicit.
 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined – safe fallback */

    /* static cache for results, growing as needed */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* current candidate of the form 36 + 43*k, start at 79 */
    static idx_t next_candidate = 79;

    /* return cached value if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* keep searching until we have at least n primes */
    while (cache_len < n) {
        if (is_prime(next_candidate)) {
            /* grow the array if full */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;   /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        next_candidate += 43;   /* next number in the arithmetic progression */
    }

    return cache[n - 1];
}
