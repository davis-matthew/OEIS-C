/*
 * A061351  Between p and the next prime either there are no numbers or there
 *          is a single squarefree number.
 * https://oeis.org/A061351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 251, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate primes in order and record prime p
 * when the next prime q satisfies:
 *   - q - p == 1   (the pair 2,3)
 *   - q - p == 2   and the middle number p+1 is squarefree.
 * The recorded primes form A061351 (offset 0: a(0)=2). */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/is_squarefree.h"

/* cached terms of the sequence */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;

/* current candidate for primality testing */
static idx_t cur_candidate = 2;
/* last prime seen */
static idx_t last_prime = 0;

/* ensure the cache has room for at least 'need' elements */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < need) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* append a new term to the cache */
static inline void append_term(seq_t val)
{
    ensure_cache_capacity(cache_len + 1);
    cache[cache_len++] = val;
}

seq_t a(idx_t n)
{
    /* return cached value if already computed */
    if ((size_t)n < cache_len)
        return cache[n];

    /* generate primes until we have the (n+1)-th term */
    while (cache_len <= (size_t)n) {
        /* advance cur_candidate to the next prime */
        while (!is_prime(cur_candidate)) cur_candidate++;

        /* cur_candidate is a prime */
        if (last_prime != 0) {
            idx_t diff = cur_candidate - last_prime;
            if (diff == 1) {                      /* 2 -> 3 */
                append_term((seq_t)last_prime);
            } else if (diff == 2) {              /* twin primes */
                seq_t middle = (seq_t)last_prime + 1;
                if (is_squarefree(middle))
                    append_term((seq_t)last_prime);
            }
        }

        /* update state for next iteration */
        last_prime = cur_candidate;
        cur_candidate++;
    }

    return cache[n];
}
