/*
 * A121478  Triangular numbers with three distinct prime factors.
 * https://oeis.org/A121478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th triangular number that has exactly three
 *            distinct prime factors
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/count_distinct_primes.h"

/* Memoised cache of found triangular numbers */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;
static idx_t  last_k     = 0;          /* last k processed */

/* Return the n-th term of A121478, memoising results as needed. */
seq_t a(idx_t n)
{
    while (cache_len < n) {
        ++last_k;

        /* Decompose k*(k+1)/2 into two coprime factors */
        idx_t a_val = last_k;
        idx_t b_val = last_k + 1;
        if (a_val & 1) {            /* k odd -> divide (k+1) by 2 */
            b_val >>= 1;
        } else {                    /* k even -> divide k by 2 */
            a_val >>= 1;
        }

        /* Count distinct prime factors of each part */
        uint64_t x1 = (uint64_t)a_val;
        uint64_t x2 = (uint64_t)b_val;
        int cnt = count_distinct_primes(x1) + count_distinct_primes(x2);

        if (cnt == 3) {
            /* Compute the triangular number using 128‑bit arithmetic to avoid overflow on the
               intermediate product before division. */
            unsigned __int128 tmp = (unsigned __int128)last_k * ((unsigned __int128)last_k + 1) / 2;
            seq_t tri = (seq_t)tmp;

            /* Grow the cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 32;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) { exit(1); }
                cache = new_cache;
                cache_cap = new_cap;
            }

            cache[cache_len++] = tri;
        }
    }
    return cache[n - 1];
}
