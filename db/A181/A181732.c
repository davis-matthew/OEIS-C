/*
 * A181732  Numbers n such that 90n + 1 is prime.
 * https://oeis.org/A181732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Numbers n such that 90n + 1 is prime
 * Range:     u8 n <= 1, u16 n <= 350, u32 n <= 62, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

static seq_t *cache = NULL;       /* cached terms of the sequence */
static idx_t  cache_size = 0;     /* number of terms stored */
static size_t cache_cap = 0;      /* allocated capacity of cache */
static idx_t  last_candidate = 0; /* last integer n tested for 90n+1 */

seq_t a(idx_t n)
{
    /* Return cached result if already computed */
    if (n <= cache_size)
        return cache[n - 1];

    /* Extend cache until we have at least n terms */
    while (cache_size < n) {
        ++last_candidate;                         /* next n candidate */

        /* Compute 90*last_candidate + 1 safely in 64‑bit */
        uint64_t test_val64 = 90ULL * last_candidate + 1ULL;
        seq_t test_val = (seq_t)test_val64;

        if (is_prime_big(test_val)) {
            /* Grow cache if needed */
            if (cache_size == cache_cap)
                ensure_capacity(cache_size + 1, &cache_cap,
                                (void **)&cache, sizeof(seq_t));

            cache[cache_size++] = (seq_t)last_candidate;  /* store the term */
        }
    }

    return cache[n - 1];
}
