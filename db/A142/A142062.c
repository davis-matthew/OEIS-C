/*
 * A142062  Primes congruent to 23 mod 33.
 * https://oeis.org/A142062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime congruent to 23 (mod 33)
 * Range:     u8 n <= 2, u16 n <= 250, u32 n <= 260, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"


seq_t a(idx_t n)
{
    /* Cache storage for primes found so far */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if (n == 0)
        return (seq_t)0;                     /* offset is 1 */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    /* Return from cache if available */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* Ensure the cache is large enough for at least n entries */
    if (n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            exit(1);                         /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Candidate value for the next prime test */
    seq_t cand;

    if (cache_len == 0) {
        /* First term is always 23 */
        cand = (seq_t)23;
        cache[cache_len++] = cand;
        if (cache_len >= n)
            return cache[n - 1];
    } else {
        cand = cache[cache_len - 1];
    }

    /* Generate primes until we have the nth one */
    while (cache_len < n) {
        cand = cand + (seq_t)33;            /* next candidate ≡ 23 (mod 33) */
        if (is_prime_big(cand)) {
            cache[cache_len++] = cand;
        }
    }

    return cache[n - 1];
}
