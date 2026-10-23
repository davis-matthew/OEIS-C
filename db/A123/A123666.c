/*
 * A123666  Numbers with an even number of prime factors, at least half of
 *          which are 2.
 * https://oeis.org/A123666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of the positive integers whose factorisation contains
   an even number of prime factors (counted with multiplicity) and at least
   half of those prime factors are 2.  The function implements a brute‑force
   search with a cache.  It is not derived from a closed formula.  */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/count_prime_factors.h"

/* Ensure that the cache array has capacity at least `needed`.  */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem)
{
    if (needed <= *cap)
        return;
    size_t new_cap = *cap ? *cap << 1 : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    *ptr = realloc(*ptr, new_cap * elem);
    *cap = new_cap;
}

/* Cache of terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 1;

/* Return true iff x satisfies the definition.  */
static bool qualifies(seq_t x)
{
    seq_t tmp   = x;
    seq_t v2    = 0;

    /* count how many 2's divide x */
    while ((tmp & 1) == 0) {
        tmp >>= 1;
        ++v2;
    }

    seq_t omega = count_prime_factors(x);      /* total number of prime factors */
    if ((omega & 1) != 0)                     /* must be even */
        return false;

    seq_t odd_part = omega - v2;
    return v2 >= odd_part;                     /* at least half are 2 */
}

seq_t a(idx_t n)
{
    if (n == 0)            /* defined only for n >= 1 */
        return 0;

    /* grow cache until we have n terms */
    for ( ; cache_size < n; ++next_candidate) {
        if (qualifies((seq_t)next_candidate)) {
            ensure_capacity(cache_size + 1, &cache_cap, (void **)&cache, sizeof(seq_t));
            cache[cache_size++] = (seq_t)next_candidate;
        }
    }
    return cache[n - 1];
}
