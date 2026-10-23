/*
 * A138641  Nonnegative integers n such that 19*n-2 is prime.
 * https://oeis.org/A138641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th positive integer m (n ≥ 1) for which
   19*m – 2 is prime.  The sequence starts with
   1, 7, 15, 19, 21, 31, … as given in OEIS A138641.  The implementation
   keeps an internal cache of found terms and expands it lazily as needed.
   The algorithm is O(n) with a constant factor of about 3–4 checks per
   term in the worst case.  It works for any seq_t and idx_t supported
   by the build environment.  No global side effects other than the cache
   are used. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;        /* array of sequence values (the m's) */
static size_t cache_len = 0;       /* number of elements currently stored */
static size_t cache_cap = 0;       /* allocated capacity */
static uint64_t next_candidate = 1;   /* next integer m to test */

/* Ensure the cache has capacity for at least 'need' elements.  The cast
   is required for C++ compilation when seq_t is a class type. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 32;
    if (new_cap < need)
        new_cap = need;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* n is 1‑based; behaviour for n < 1 is undefined. */
    if (n == 0)
        return 0;   /* placeholder for invalid index */

    /* Lazily extend the cache until we have at least n entries. */
    while (cache_len < (size_t)n) {
        uint64_t m = next_candidate;
        uint64_t val = 19 * m - 2;
        if (is_prime_big((seq_t)val)) {
            /* Store m in the cache. */
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)m;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
