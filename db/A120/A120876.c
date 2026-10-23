/*
 * A120876  (Product of twin primes - 1)/2.
 * https://oeis.org/A120876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p * (p + 2) - 1) / 2 where (p, p+2) is the nth
 *            twin‑prime pair
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 988, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation caches computed values so that calls can be made in any order. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *cache        = NULL;   /* cached a(1), a(2), ... */
static size_t cache_cap    = 0;      /* allocated capacity of cache */
static idx_t  found_count  = 0;      /* number of twin‑prime pairs found so far */
static idx_t  next_candidate = 3;    /* next odd integer to test for twin prime lower bound */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined for n < 1, return 0 */

    /* Ensure the cache can hold at least n values. */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(*cache));
        if (!tmp) abort();           /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Produce twin‐prime pairs until enough values are cached. */
    while (found_count < n) {
        if (is_prime(next_candidate) && is_prime(next_candidate + (idx_t)2)) {
            seq_t prod = (seq_t)next_candidate *
                         ((seq_t)next_candidate + (seq_t)2);
            cache[found_count] = (prod - (seq_t)1) / (seq_t)2;
            ++found_count;
        }
        next_candidate += (idx_t)2;   /* move to next odd integer */
    }

    return cache[n - 1];
}
