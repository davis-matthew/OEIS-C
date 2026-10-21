/*
 * A085704  Primes of the form prime(k)*2 - prime(k-1), k>1.
 * https://oeis.org/A085704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(k)*2 − prime(k‑1) (k > 1)
 * Range:     u8 n <= 7, u16 n <= 482, u32 n <= 483, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   is_prime_big, next_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"
#include "../utils/seq_realloc.h"

/* External definition of seq_t and idx_t are provided by the build system. */
seq_t a(idx_t n)
{
    /* Offset is 1; n < 1 is out‑of‑bounds – return 0 just in case. */
    if (n == 0) return (seq_t)0;

    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    /* Keep the most recent pair of consecutive primes. */
    static uint64_t prev_prime = 2;   /* prime(1) */
    static uint64_t curr_prime = 3;   /* prime(2) */

    /* Grow the cache once before use. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = seq_realloc(NULL, 0, cache_cap);
    }

    /* Generate terms until the n‑th one is available. */
    while (cache_len < n) {
        /* Compute the candidate value 2*curr_prime – prev_prime. */
        uint64_t temp = 2ull * curr_prime - prev_prime;
        if (is_prime_big((seq_t)temp)) {
            /* Append to cache if it is a new term. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)temp;
        }

        /* Advance to the next pair of primes. */
        prev_prime = curr_prime;
        curr_prime = next_prime(curr_prime + 1);
    }

    return cache[n - 1];
}
