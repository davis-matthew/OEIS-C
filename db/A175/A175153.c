/*
 * A175153  Numbers k such that 13*k-1, 13*k+1 are twin primes.
 * https://oeis.org/A175153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 199, u32 n <= 47, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th integer k such that 13*k-1 and 13*k+1 are twin primes. */
/* The function searches over positive integers k, checks primality of 13*k±1,
   and caches found values to answer future calls efficiently. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

static seq_t *cache = NULL;   /* Cached sequence values */
static size_t cache_cap = 0;  /* Current allocation capacity of cache */
static idx_t cache_len = 0;   /* Number of values already computed */

static seq_t last_k = 0;      /* Largest k found so far (in cache) */

/* Return the n-th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    /* Return cached values when already computed. */
    if (cache_len >= n)
        return cache[n - 1];

    /* Start searching from the next multiple of 6 after the last found k. */
    seq_t k = (cache_len == 0) ? (seq_t)6 : last_k + (seq_t)6;

    while (cache_len < n) {
        /* Compute 13 * k and check twin primes. */
        seq_t pk = (seq_t)13 * k;
        seq_t p1 = pk - (seq_t)1;   /* 13*k - 1 */
        seq_t p2 = pk + (seq_t)1;   /* 13*k + 1 */

        if (is_prime_big(p1) && is_prime_big(p2)) {
            /* Store the found k in the cache. */
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = k;
            last_k = k;
        }

        k += (seq_t)6; /* Next candidate k must be multiple of 6 */
    }

    return cache[n - 1];
}
