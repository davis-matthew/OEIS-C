/*
 * A209494  Smallest prime p such that 2p*n^2 - 1 is prime, or 0 if no such
 *          prime exists.
 * https://oeis.org/A209494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p such that 2·p·n² – 1 is prime, or 0 if
 *            none exists
 * Range:     u8 n <= 8, u16 n <= 30, u32 n <= 39, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"

/* Cache for previously computed values. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static idx_t cached_up_to = 0;

/* Return the n‑th term of sequence A209494. */
seq_t a(idx_t n)
{
    /* Grow the cache to hold values up to n. */
    if ((idx_t)(cache_len) <= n) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)           /* memory allocation failure */
            return (seq_t)0;
        cache = tmp;
        /* Initialise new entries to 0. */
        for (idx_t i = (idx_t)cache_len; i <= n; ++i)
            cache[i] = (seq_t)0;
        cache_len = new_len;
    }

    /* Compute any missing values. */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        uint64_t p = 2;               /* first prime */
        seq_t  res = (seq_t)0;         /* 0 means no prime found yet */

        /* Limit the search to avoid infinite loops for pathological n. */
        const uint64_t PRIME_LIMIT = 5000000ULL;

        while (1) {
            seq_t i_sq     = (seq_t)i * (seq_t)i;
            seq_t candidate = ((seq_t)2 * (seq_t)p * i_sq) - (seq_t)1;

            if (is_prime_big(candidate)) {
                res = (seq_t)p;
                break;
            }
            if (p > PRIME_LIMIT) {    /* give up – return 0 */
                res = (seq_t)0;
                break;
            }
            p = next_prime(p);
        }
        cache[i] = res;
    }

    cached_up_to = n;
    return cache[n];
}
