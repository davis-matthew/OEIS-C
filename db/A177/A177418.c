/*
 * A177418  Primes of the form 10^k - 59.
 * https://oeis.org/A177418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10^A108506(n) - 59, i.e. the n‑th prime of the form 10^k
 *            − 59 (k ≥ 2)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* static cache of found primes */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of primes in cache   */
    static size_t cache_cap  = 0;   /* capacity of cache          */

    /* exponent to test next */
    static idx_t next_k = 1;          /* will be incremented before testing, so first test is k=2 */

    /* Grow cache when needed (simple doubling strategy) */
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Find primes until we have at least n of them */
    while (cache_size < (size_t)n) {
        ++next_k; /* move to next exponent */

        /* candidate = 10^next_k - 59 */
        seq_t candidate = pow10(next_k) - (seq_t)59;

        if (is_prime_big(candidate)) {
            if (cache_size == cache_cap) {      /* enlarge cache */
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) { /* allocation failed; abort */ abort(); }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = candidate;
        }
    }

    return cache[(size_t)n - 1];
}
