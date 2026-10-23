/*
 * A173416  Exactly one of 2n^2-1 and 2n^2+1 is prime.
 * https://oeis.org/A173416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of A173416: the nth positive integer n for which
 * exactly one of 2n² – 1 and 2n² + 1 is prime.  The function memoises
 * all previously computed terms so that calls in any order are efficient. */

/* Required helper for 64‑bit primality testing. */
#include "../utils/is_prime_small.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* memoised terms */
static size_t cache_len = 0;         /* number of terms in cache */
static size_t cache_cap = 0;         /* allocated capacity */
static idx_t last_k = 0;             /* largest k already examined */

/* Append a new term to the cache, reallocating as necessary. */
static void cache_push(seq_t val)
{
    if (cache_len == cache_cap) {
        cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = val;
}

/* Public entry point required by the build system. */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n entries. */
    while (cache_len < (size_t)n) {
        ++last_k;
        uint64_t k = (uint64_t)last_k;

        /* Compute 2k²–1 and 2k²+1 as 64‑bit integers. */
        uint64_t v1 = 2ULL * k * k - 1;
        uint64_t v2 = 2ULL * k * k + 1;

        bool p1 = is_prime_small(v1);
        bool p2 = is_prime_small(v2);

        /* Store k if exactly one of the two numbers is prime. */
        if (p1 ^ p2) {
            cache_push((seq_t)k);
        }
    }
    return cache[n - 1];
}
