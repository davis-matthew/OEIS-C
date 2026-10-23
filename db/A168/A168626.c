/*
 * A168626  Numbers n such that n and n+-1 have 3 or more distinct prime
 *          factors.
 * https://oeis.org/A168626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A168626: Numbers n such that n-1, n and n+1 each have at least three
 * distinct prime factors.  The function returns the n‑th such integer
 * (index starts at 1).  It uses a simple incremental search with
 * caching.  The helper function count_distinct_primes is supplied
 * by the utilities. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* Static cache of found terms: cache[0] = a(1) */
    static seq_t *cache = NULL;
    static size_t size  = 0;
    static size_t cap   = 0;
    static idx_t  nxt   = 2;        /* next integer to test (>=2) */

    /* Ensure we have at least n terms cached. */
    if (size < n) {
        /* Grow the cache array if needed. */
        if (n > cap) {
            size_t new_cap = cap ? cap << 1 : 64;
            while (new_cap < n) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();              /* allocation failure */
            cache = tmp;
            cap   = new_cap;
        }

        /* Find additional terms until we have n of them. */
        while (size < n) {
            /* For n=2, nxt-1 = 1; count_distinct_primes(1) works
             * (returns 0).  We only accept numbers where all three
             * consecutive integers have >=3 distinct prime factors. */
            if (count_distinct_primes(nxt - 1) >= 3 &&
                count_distinct_primes(nxt)     >= 3 &&
                count_distinct_primes(nxt + 1) >= 3)
            {
                cache[size++] = (seq_t)nxt;
            }
            ++nxt;
        }
    }
    return cache[n - 1];
}
