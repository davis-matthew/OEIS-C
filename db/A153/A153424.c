/*
 * A153424  Primes of the form k^2+3*k+223.
 * https://oeis.org/A153424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 142, u32 n <= 46, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   free_seq_array, is_prime_big, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form k^2 + 3*k + 223.
   a(n) is the n‑th prime value of that form (a(1)=223). */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Cache of found values. */
    static seq_t *cache = NULL;
    static size_t  cache_count = 0;
    static size_t  cache_cap   = 0;
    static idx_t   last_k      = 0;          /* next k to test */

    /* Grow the cache if necessary. */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *tmp = seq_alloc_new_array(new_cap);
        if (cache) free_seq_array(cache);
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Generate further terms until the cache holds n items. */
    while (cache_count < (size_t)n) {
        idx_t k = last_k;
        seq_t val = (seq_t)k * (seq_t)k + (seq_t)3 * (seq_t)k + (seq_t)223;
        if (is_prime_big(val))
            cache[cache_count++] = val;
        last_k = k + 1;
    }

    return cache[n - 1];
}
