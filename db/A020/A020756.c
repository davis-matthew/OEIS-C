/*
 * A020756  Numbers that are the sum of two triangular numbers.
 * https://oeis.org/A020756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 157, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Search-based implementation of A020756:
    a(n) = the nth nonnegative integer that can be written as a sum of two
           triangular numbers.  A number m is in the sequence iff 4*m+1
           has no prime factor ≡ 3 (mod 4) with odd exponent.  The function
           builds a cache on demand and is order‑independent.  Complexity
           is O(N) for the (Nth) term, which is well below 2 s for N≃10000. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache   = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t  last_checked = 0;   /* next integer to test (starting at 0) */

    /* Extend the cache until it holds at least n terms */
    while (cache_size < (size_t)n) {
        /* Test whether last_checked can be expressed as sum of two triangular numbers */
        bool ok = true;
        uint64_t val = (uint64_t)last_checked * 4 + 1;  /* 4*m+1 */
        uint64_t primes[64], exps[64];
        size_t pcnt;
        factor_u64(val, primes, exps, &pcnt);

        for (size_t i = 0; i < pcnt; ++i) {
            if (primes[i] % 4 == 3 && (exps[i] & 1)) {
                ok = false;
                break;
            }
        }

        if (ok) {
            /* Grow cache if needed */
            if (cache_size == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 4;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = (seq_t)last_checked;
        }
        ++last_checked;
    }

    return cache[n - 1];
}
