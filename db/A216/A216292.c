/*
 * A216292  Values of k such that there is exactly one prime between 10k and
 *          10k + 9.
 * https://oeis.org/A216292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A216292: a(n) = the n-th integer k such
   that there is exactly one prime in the interval [10k, 10k+9].
   The function builds values on demand, caching results. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Static cache of found values.  k_cache holds the first cache_size
   elements of the sequence.  cache_cap is the number of elements
   allocated.  next_k is the smallest k that has not yet been tested. */
static seq_t *k_cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;
static idx_t next_k = 1;

/* Grow the cache array to include at least 'needed' elements. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(k_cache, new_cap * sizeof(seq_t));
    if (!new_arr)           /* Allocation failure – terminate. */
        exit(1);
    k_cache = new_arr;
    cache_cap = new_cap;
}

/* External function being requested. */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for n = 0; return sentinel */
        return 0;

    /* Compute and cache values until we have at least n terms. */
    while (cache_size < (size_t)n) {
        int prime_cnt = 0;
        for (idx_t offset = 0; offset < 10; ++offset) {
            idx_t num = 10 * next_k + offset;
            if (is_prime(num)) {
                ++prime_cnt;
                if (prime_cnt > 1)
                    break;
            }
        }
        if (prime_cnt == 1) {          /* one prime only -> record k */
            ensure_cache_cap(cache_size + 1);
            k_cache[cache_size++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return k_cache[(size_t)n - 1];
}
