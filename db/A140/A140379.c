/*
 * A140379  Nonprimes of the form 14k+1.
 * https://oeis.org/A140379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Non‑prime numbers of the form 14k+1 (OEIS A140379).  The function enumerates
 * such numbers by scanning successive k and rejecting primes.  A small cache
 * keeps already found terms so that calls in any order remain fast.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/isPrimeTrial.h"

/* Cache for memoisation.  cache[0] holds the first term (1). */
static seq_t   *cache      = NULL;
static size_t  cache_len  = 0;   /* number of elements currently cached   */
static size_t  cache_cap  = 0;   /* allocated capacity of the cache array */
static idx_t   next_k     = 0;   /* next k (in 14k+1) to test              */

/* Grow the cache array to hold at least ‘need’ elements. */
static void ensure_cache_cap(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed – abort as the test harness cannot handle it. */
        abort();
    }
    cache   = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* Undefined for n < 1 – return 0 to avoid UB. */

    /* Extend the cache until we have at least n terms. */
    while (cache_len < n) {
        /* Compute the next candidate: 14 * k + 1.   Use uint64_t intermediate
         * to avoid overflow of the multiplication for very large k.          */
        uint64_t raw_val = 14ULL * (uint64_t)next_k + 1ULL;
        seq_t val = (seq_t)raw_val;          /* cast to output type */

        /* Include 1 and every composite number; prime numbers are skipped. */
        if (val == (seq_t)1 || !isPrimeTrial(val)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = val;
        }
        ++next_k;
    }
    return cache[n - 1];
}
