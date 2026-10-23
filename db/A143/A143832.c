/*
 * A143832  Primes of the form 14 n^2-1.
 * https://oeis.org/A143832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 18, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of the form 14 n² - 1 (OEIS A143832) */
/* The function builds the sequence by testing successive n starting at 1
 * and storing each prime found.  Cached results allow out‑of‑order calls. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

static seq_t *cache = NULL;      /* cache of found primes */
static idx_t cache_len = 0;      /* number of primes stored */
static size_t cache_cap = 0;     /* current capacity of cache */
static idx_t next_i = 1;         /* next n to test for 14 n^2‑1 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset is 1; safeguard */

    /* Generate primes until we have n of them. */
    while (cache_len < n) {
        /* 14·n² – 1 using seq_t arithmetic to avoid overflow on big types */
        seq_t cand = 14 * ((seq_t)next_i * (seq_t)next_i) - 1;

        if (is_prime_big(cand)) {
            /* Ensure enough space in the cache. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                ensure_capacity(cache_len + 1, &cache_cap,
                                (void **)&cache, sizeof(seq_t));
            }
            cache[cache_len++] = cand;
        }
        ++next_i;
    }

    return cache[n - 1];
}
