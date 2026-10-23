/*
 * A142924  Primes congruent to 62 mod 63.
 * https://oeis.org/A142924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 187, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the n-th prime p with p ≡ 62 (mod 63) */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;          /* array of discovered terms   */
static size_t cache_cap = 0;         /* allocated capacity          */
static size_t cache_len = 0;         /* number of cached terms      */
static idx_t next_candidate = 251;   /* start at the first prime    */

/* Append a value to the cache, growing it if necessary */
static void append_to_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failure            */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Ensure that we have cached at least n terms (1‑based index) */
static void ensure_cache(idx_t n)
{
    while (cache_len < (size_t)n) {
        if (is_prime(next_candidate)) {            /* primality test */
            append_to_cache((seq_t)next_candidate); /* store in cache   */
        }
        next_candidate += 63;                      /* next candidate ≡62 mod 63 */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n < 1, safe return */
    ensure_cache(n);                          /* generate up to the n‑th prime     */
    return cache[n - 1];                       /* 0‑based indexing in cache         */
}
