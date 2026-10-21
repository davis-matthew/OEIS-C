/*
 * A051649  Primes of the form 2310*p + 1 where p is a prime.
 * https://oeis.org/A051649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2310*p + 1 where p is prime
 * Range:     u8 none, u16 n <= 4, u32 n <= 4, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"   /* is_prime_small(uint64_t) */
#include "../utils/is_prime_big.h"     /* is_prime_big(seq_t) */

/* Static cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;

/* Resize the cache if needed */
static void ensure_cache_capacity(void) {
    if (cache_len >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* Return the n-th term of OEIS sequence A051649 (1‑based) */
seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* undefined for offset < 1 */

    if (n <= cache_len) return cache[n-1];    /* Cached term */

    /* Determine starting prime p to search from */
    idx_t p_start = 2;
    if (cache_len > 0) {
        seq_t last_val = cache[cache_len-1];
        seq_t last_p_seq = (last_val - 1) / 2310;
        p_start = ((idx_t)last_p_seq) + 1;
    }

    /* Search for the next terms until n is reached */
    for (idx_t p = p_start; ; ++p) {
        if (!is_prime_small(p)) continue;       /* Skip non‑primes quickly */

        seq_t candidate = (seq_t)2310 * (seq_t)p + 1;
        if (is_prime_big(candidate)) {           /* Verify 2310*p+1 is prime */
            ensure_cache_capacity();
            cache[cache_len++] = candidate;
            if (cache_len >= n) break;
        }
    }

    return cache[n-1];
}
