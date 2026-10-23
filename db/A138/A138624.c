/*
 * A138624  Nonnegative integers k such that 17*k+5 is prime.
 * https://oeis.org/A138624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 567, u32 n <= 56, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Nonnegative integers k such that 17*k+5 is prime.  The function
   a(idx_t n) returns the (n‑th) element of this sequence with
   offset 1.  It uses incremental search, testing each candidate
   with the provided is_prime_big() routine and memoises the
   results so that calls in any order are efficient. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Cache of already found terms.
   cache_len   – number of terms stored
   cache_cap   – allocated capacity
   next_k      – smallest k not yet examined        */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;
static seq_t next_k = 0;

seq_t a(idx_t n)
{
    if (n == 0)       /* behaviour below offset is irrelevant */
        return 0;

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n) {
        /* Test candidate next_k : 17*next_k + 5 */
        seq_t val = (seq_t)17 * next_k + (seq_t)5;
        if (is_prime_big(val)) {
            /* add next_k to cache */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();              /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_k;
        }
        next_k++;
    }

    return cache[n - 1];
}
