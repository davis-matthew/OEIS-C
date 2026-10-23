/*
 * A158293  Primes whose digit sum is a multiple of 10.
 * https://oeis.org/A158293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   digit_sum, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th prime whose digit sum is a multiple of 10. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/is_prime.h"
#include "../utils/digit_sum.h"

/* Cached sequence of primes satisfying the digit–sum condition. */
static seq_t *cache      = NULL;   /* array of stored terms      */
static size_t cache_len  = 0;      /* number of terms stored     */
static size_t cache_cap  = 0;      /* current allocation capacity*/
static idx_t next_candidate = 2;   /* integer to test next       */

/* Ensure the cache can hold at least 'need' terms. */
static void
ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t newcap = cache_cap ? (cache_cap << 1) : 16;
    while (newcap < need)
        newcap <<= 1;
    seq_t *newptr = (seq_t *)malloc(newcap * sizeof(seq_t));
    if (!newptr)
        return;  /* allocation failure – unlikely for required sizes   */
    if (cache) {
        memcpy(newptr, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    cache = newptr;
    cache_cap = newcap;
}

/* Extend the cache so that it contains at least 'n' terms. */
static void
extend_cache(idx_t n)
{
    if ((size_t)n <= cache_len)
        return;
    ensure_cache_capacity((size_t)n);
    while (cache_len < (size_t)n) {
        /* Find the next prime with digit sum a multiple of 10. */
        while (!is_prime(next_candidate) ||
               (digit_sum((seq_t)next_candidate) % (seq_t)10) != 0)
            ++next_candidate;
        cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;            /* prepare for the next search   */
    }
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;      /* undefined below the offset   */
    extend_cache(n);
    return cache[(size_t)n - 1];
}
