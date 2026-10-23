/*
 * A033868  Numbers n such that 7*n-11 is prime.
 * https://oeis.org/A033868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Numbers n such that 7*n-11 is prime (OEIS A033868)
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

/* Cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;
static idx_t  last_candidate = 1;   /* next integer to test */

/* Return the n-th element (1‑based) of the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset handling – not called for n < 1 */

    /* Initialise cache on first call */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (cache_size < n) {
        last_candidate++;                     /* next candidate m */
        uint64_t test = 7ull * (uint64_t)last_candidate - 11ull;  /* 7*m-11 */

        /* Test primality of test (fits in 64‑bit) */
        if (is_prime_small(test)) {
            /* Grow cache if necessary */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap << 1;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            /* Store the candidate itself as a seq_t value */
            cache[cache_size++] = (seq_t)last_candidate;
        }
    }

    return cache[n - 1];
}
