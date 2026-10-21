/*
 * A075280  Interprimes which are of the form s*prime, s=5.
 * https://oeis.org/A075280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   is_prime_big, next_prime, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th Interprime of the form 5*prime.
   Search for pairs of consecutive primes (p,q), take
   m = (p+q)/2 and keep those m that are divisible by 5
   with m/5 also prime.  The first such m is 15. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static uint64_t prev_prime = 0;
static uint64_t next_prime_val = 0;
static bool initialized = false;

/* Grow the cache to hold at least min_cap elements. */
static void grow_cache(size_t min_cap)
{
    if (cache_cap < min_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        if (new_cap < min_cap) new_cap = min_cap;
        seq_t *new_arr = seq_alloc_new_array(new_cap);
        if (cache && cache_len) {
            memcpy(new_arr, cache, cache_len * sizeof(seq_t));
        }
        if (cache) seq_free_array(cache);
        cache = new_arr;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        initialized = true;
        prev_prime = 3ULL;
        next_prime_val = 5ULL;
    }

    if (cache_len >= (size_t)n) {
        return cache[(size_t)n - 1];
    }

    while (cache_len < (size_t)n) {
        seq_t m = ((seq_t)prev_prime + (seq_t)next_prime_val) / 2;

        if (m % (seq_t)5 == 0) {
            seq_t q = m / (seq_t)5;
            if (is_prime_big(q)) {
                grow_cache(cache_len + 1);
                cache[cache_len++] = m;
            }
        }

        prev_prime = next_prime_val;
        next_prime_val = next_prime(prev_prime);
    }

    return cache[(size_t)n - 1];
}
