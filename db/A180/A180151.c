/*
 * A180151  Numbers k such that k and k + 2 are both divisible by exactly five
 *          primes (counted with multiplicity).
 * https://oeis.org/A180151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 870, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 59 terms through n = 9674 (2026-11-01)
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A180151: numbers k such that k and k + 2 each have exactly five
   prime factors (counted with multiplicity). */
/* Implementation: iteratively search for qualifying numbers with memoisation. */

#include <stdlib.h>
#include "../utils/bigomega.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* stored terms */
    static size_t cap   = 0;      /* capacity of cache   */
    static size_t size  = 0;      /* number of terms stored */
    static idx_t  cand  = 1;      /* next candidate to test */

    /* grow the cache if we still need more terms */
    if (size < n) {
        if (cap < n) {
            size_t new_cap = cap ? cap * 2 : 16;
            if (new_cap < n) new_cap = n;
            cache = seq_realloc(cache, cap, new_cap);
            cap   = new_cap;
        }

        /* examine successive candidates until we find enough terms */
        while (size < n) {
            if (bigomega(cand) == 5 && bigomega(cand + 2) == 5) {
                cache[size++] = cand;
            }
            ++cand;
        }
    }

    return cache[n - 1];
}
