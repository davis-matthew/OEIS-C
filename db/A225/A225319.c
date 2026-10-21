/*
 * A225319  Prime numbers p such that p - (product of digits of p) is also
 *          prime.
 * https://oeis.org/A225319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th prime p such that p - product_of_digits(p) is
 *            also prime
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   digit_product, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: incremental search over primes using Miller‑Rabin test supplied by is_prime_big   */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_product.h"
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;          /* cached sequence values */
static size_t cache_size = 0;        /* number of cached values */
static size_t cache_cap   = 0;        /* capacity of cache */
static uint64_t cur = 2;             /* current integer being tested for primality */

/* Ensure the cache can hold at least 'need' elements */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap < need) new_cap *= 2;
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n < 1 */

    /* Generate terms until we have at least n */
    while (cache_size < (size_t)n) {
        /* Find next prime */
        while (!is_prime_big((seq_t)cur)) cur++;

        /* Evaluate the condition for this prime */
        seq_t p = (seq_t)cur;
        seq_t prod = digit_product(p);
        seq_t diff = p - prod;
        if (diff > 0 && is_prime_big(diff)) {
            ensure_cache(cache_size + 1);
            cache[cache_size++] = p;
        }
        cur++;    /* move to the next integer for future searches */
    }

    return cache[n - 1];
}
