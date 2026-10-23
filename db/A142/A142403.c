/*
 * A142403  Primes congruent to 17 mod 48.
 * https://oeis.org/A142403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 419, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes ≡ 17 (mod 48), found with incremental search */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t *prime_cache = NULL;
static idx_t cache_size  = 0;
static idx_t cache_cap   = 0;
static uint64_t next_candidate = 17;   /* first number to test */

/* Ensure the cache can hold at least 'needed' elements */
static void grow_cache(idx_t needed)
{
    if (cache_cap < needed) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 64;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *new_ptr = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
        if (!new_ptr && new_cap != 0) abort();
        prime_cache = new_ptr;
        cache_cap   = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but safe */

    /* Generate primes until we have at least n of them */
    while (cache_size < n) {
        uint64_t cand = next_candidate;
        while (1) {
            if (is_prime((idx_t)cand)) {
                grow_cache(cache_size + 1);
                prime_cache[cache_size++] = (seq_t)cand;
                next_candidate = cand + 48;
                if (cache_size == n) break;
            }
            cand += 48;
        }
    }
    return prime_cache[n - 1];
}
