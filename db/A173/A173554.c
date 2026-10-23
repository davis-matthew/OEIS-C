/*
 * A173554  Primes of form 5+38*n^2.
 * https://oeis.org/A173554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 29, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth prime of the form 5 + 38*k^2  (k = 0,1,2,…) */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache   = NULL;   /* stored primes */
static size_t cache_size = 0;   /* number of cached primes */
static size_t cache_cap  = 0;   /* allocated capacity of cache */
static idx_t  last_k     = 0;   /* next k to try */

/* Ensure the cache can hold at least 'needed' elements */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                       /* exit on allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the nth term (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 */

    /* Return from cache if already computed. */
    if ((size_t)n <= cache_size) return cache[n - 1];

    /* Generate new terms until the nth is found. */
    while (cache_size < (size_t)n) {
        seq_t kseq = (seq_t)last_k;
        seq_t val  = 5 + (seq_t)38 * kseq * kseq;   /* candidate 5 + 38*k^2 */

        if (is_prime_big(val)) {
            ensure_capacity(cache_size + 1);
            cache[cache_size++] = val;
        }
        last_k++;    /* advance to next k for next iteration */
    }

    return cache[n - 1];
}
