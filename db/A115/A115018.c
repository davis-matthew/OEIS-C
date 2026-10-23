/*
 * A115018  Numbers k such that (k+1)*(k+2)^2 + 1 is prime.
 * https://oeis.org/A115018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 17, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that (k+1)*(k+2)^2 + 1 is prime.
   a(1) = 0, a(2) = 1, a(3) = 3, ...
   The function returns the nth such number (1‑based). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"  /* provides is_prime_big(seq_t) */

static seq_t *cache = NULL;      /* array of found k values */
static size_t cache_len = 0;     /* number of elements in the cache */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static seq_t last_k = 0;         /* last k value stored in the cache */

/* Extend the cache if needed. */
static void ensure_cache_capacity(size_t min_cap)
{
    if (cache_cap >= min_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < min_cap) new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) exit(EXIT_FAILURE);   /* unrecoverable allocation error */
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Compute the nth element of A115018. */
seq_t a(idx_t n)
{
    if (n == 0) { /* undefined below offset, but return 0 to avoid crash */
        return 0;
    }
    if (cache_len >= (size_t)n)
        return cache[(size_t)n - 1];

    /* Start searching from the last found k + 1, or from 0 if cache empty. */
    idx_t k = (cache_len == 0) ? 0 : (idx_t)(last_k + 1);

    while (cache_len < (size_t)n) {
        seq_t k_seq = (seq_t)k;
        seq_t f = ((k_seq + 1) * ((k_seq + 2) * (k_seq + 2))) + 1;
        if (is_prime_big(f)) {
            /* Store the new k value. */
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len] = k_seq;
            last_k = k_seq;
            cache_len++;
        }
        k++;
    }
    return cache[(size_t)n - 1];
}
