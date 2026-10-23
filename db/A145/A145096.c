/*
 * A145096  Primes of the form 4*k^2 - 38*k + 131.
 * https://oeis.org/A145096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime value of the quadratic 4*k^2 - 38*k + 131
 *            (k starts at 0)
 * Range:     u8 n <= 13, u16 n <= 97, u32 n <= 69, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Static cache to store already found primes of the form 4*k^2 - 38*k + 131. */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* number of primes stored in cache     */
static idx_t cache_cap  = 0;   /* current capacity of cache array       */
static idx_t next_k     = 0;   /* the next k value to test for primes   */

static void ensure_cache_capacity(idx_t required)
{
    if (required <= cache_cap)
        return;
    /* Grow cache: double size until we exceed required capacity */
    idx_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap < required)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp) {
        /* In a real project we would handle out‑of‑memory here.  */
        abort();
    }
    for (idx_t i = 0; i < cache_size; ++i)
        tmp[i] = cache[i];
    free(cache);
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Return the result if it is already cached. */
    if (n <= cache_size)
        return cache[n - 1];

    /* Generate primes until we have at least n of them. */
    for (idx_t k = next_k; ; ++k) {
        seq_t kk = (seq_t)k;
        seq_t val = (seq_t)4 * kk * kk - (seq_t)38 * kk + (seq_t)131;
        if (is_prime_big(val)) {
            ensure_cache_capacity(cache_size + 1);
            cache[cache_size++] = val;
            if (cache_size >= n) {
                next_k = k + 1;          /* next search starts after current k */
                break;
            }
        }
    }
    return cache[n - 1];
}
