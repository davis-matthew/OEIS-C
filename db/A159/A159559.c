/*
 * A159559  Lexicographically first strictly increasing sequence starting a(2)
 *          = 3 with the property that a(n) is prime if and only if n is
 *          prime.
 * https://oeis.org/A159559
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m > a(n-1) such that m is prime iff n is prime,
 *            with a(2) = 3
 * Range:     u8 n <= 150, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"

/* Cache of computed sequence values */
static seq_t *seq_cache = NULL;
static idx_t cache_cap = 0;   /* allocated capacity (size_t indices) */
static idx_t cache_size = 0;  /* highest index that has been computed */

/* Grow the cache so that index 'idx' is valid for write */
static void ensure_cache(idx_t idx)
{
    if (idx < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap <= idx)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;  /* not defined for n < 2 */

    ensure_cache(n);

    /* Initialise the very first value a(2)=3 if not already done */
    if (cache_size < 2) {
        seq_cache[2] = (seq_t)3;
        cache_size = 2;
    }

    for (idx_t k = cache_size + 1; k <= n; ++k) {
        seq_t prev = seq_cache[k - 1];
        seq_t cur  = prev + (seq_t)1;

        if (is_prime(k)) {        /* need a prime */
            while (!is_prime_big(cur))
                ++cur;
        } else {                  /* need a composite */
            while (is_prime_big(cur))
                ++cur;
        }

        seq_cache[k] = cur;
    }

    cache_size = n;
    return seq_cache[n];
}
