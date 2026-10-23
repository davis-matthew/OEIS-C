/*
 * A158217  Primes whose digit sum is a single-digit prime.
 * https://oeis.org/A158217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   digit_sum, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A158217: Primes whose digit sum is a single-digit prime. */

#include <stddef.h>
#include <stdbool.h>

#include "../utils/get_prime.h"
#include "../utils/digit_sum.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_prime_index = 0;  /* next index to fetch from get_prime (1‑based) */

seq_t a(idx_t n)
{
    /* Return the nth term, generating primes on demand. */
    if (n == 0) {
        return 0;  /* undefined for n==0, but harmless; offset is 1 */
    }
    /* Grow cache until we have at least n terms. */
    while (cache_len < (size_t)n) {
        /* Get next prime. */
        ++next_prime_index;
        seq_t p = (seq_t)get_prime(next_prime_index);
        /* Test digit sum. */
        seq_t s = digit_sum(p);
        if (s == 2 || s == 3 || s == 5 || s == 7) {
            /* Ensure capacity. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { /* allocation failed – abort gracefully */
                    return 0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = p;
        }
    }
    return cache[n - 1];
}
