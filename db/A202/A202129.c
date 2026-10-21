/*
 * A202129  Numbers n such that 90n + 71 is prime.
 * https://oeis.org/A202129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   ensure_capacity, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS: A202129
 * Description: Numbers n for which 90n + 71 is prime.
 * Sequence offset: 1  (a(1) = 0)
 * This code lazily computes the terms using a simple sieve-like scan
 * and memoises them so that any order of calls works.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;      /* allocated capacity */
    static size_t cache_len = 0;      /* number of terms stored */
    static idx_t next_candidate = 0;  /* next n to test */

    /* if we already have the term return it */
    if (cache_len >= n)
        return cache[n - 1];

    /* grow cache if required */
    if (cache_cap < (size_t)n)
        ensure_capacity((size_t)n, &cache_cap, (void **)&cache, sizeof(seq_t));

    /* scan candidates until enough terms have been gathered */
    while (cache_len < n) {
        /* compute 90*candidate + 71 as seq_t to avoid overflow in idx_t */
        seq_t val = (seq_t)90 * (seq_t)next_candidate + 71;
        if (isPrimeGeneric(val)) {
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
