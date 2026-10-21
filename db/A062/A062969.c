/*
 * A062969  Numbers k such that abs(d(k)-k-1) is prime, where d(k) is the
 *          number of divisors of k.
 * https://oeis.org/A062969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   divisor_count_u64, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A062969 – Numbers k such that abs(d(k)-k-1) is prime.
   Memoised generation of the n‑th term (offset 1). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/is_prime_big.h"

/* Static cache of found terms */
static seq_t *cache      = NULL;   /* pointer to the array of terms          */
static size_t cache_cap  = 0;     /* allocated elements                     */
static size_t cache_size = 0;     /* number of valid terms in the cache    */
static idx_t  last_checked = 0;   /* last integer k that was examined      */

/* Grow the cache array to accommodate more terms.  */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Ensure that at least n terms are available in the cache. */
static void ensure_nterms(idx_t n)
{
    while (cache_size < (size_t)n) {
        ++last_checked;                      /* examine next integer k     */
        /* Count divisors of k (k fits in uint64_t). */
        uint64_t k      = (uint64_t)last_checked;
        uint64_t d      = divisor_count_u64(k);
        uint64_t diff   = 0; /* abs(d - (k+1)) */
        if (d > k + 1)
            diff = d - (k + 1);
        else
            diff = (k + 1) - d;
        /* Test primality of the difference. */
        if (is_prime_big((seq_t)diff)) {
            if (cache_size >= cache_cap) grow_cache();
            cache[cache_size++] = (seq_t)k;
        }
    }
}

/* Return the n-th term of the sequence.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* out of defined offset range      */
    ensure_nterms(n);
    return cache[(size_t)n - 1];
}
