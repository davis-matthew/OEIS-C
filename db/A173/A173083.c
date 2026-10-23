/*
 * A173083  Semiprimes q such that q^2+3 and q^2-3 are also semiprimes.
 * https://oeis.org/A173083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the n‑th semiprime q such that q^2–3 and q^2+3 are also semiprimes.
   The function expands a static array lazily, storing already found terms
   and extending it until it contains at least n elements.  It relies on
   the helper bigomega() to test for semiprimality (bigomega(x)=2).  All
   memory allocation is performed with realloc(), which is C‑style casted
   so it compiles cleanly under both C11 and C++17.  The algorithm
   performs a brute force search; it is fast enough for n up to about
   10 000, as required. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* static cache of results already found */
    static seq_t *cache = NULL;   /* array of found q values */
    static size_t cache_size = 0; /* number of terms stored */
    static size_t cache_cap  = 0; /* allocated capacity */
    static uint64_t last_q = 1;   /* last candidate examined */

    /* extend the cache until it contains at least n elements */
    while (cache_size < n) {
        uint64_t q = last_q + 1;          /* next candidate q */
        for (;;) {
            /* q must be a semiprime */
            if (bigomega(q) == 2) {
                /* compute q^2 ± 3 as seq_t values */
                seq_t qsq   = (seq_t)q * q;
                seq_t minus = qsq - 3;
                seq_t plus  = qsq + 3;

                /* q^2±3 must be >1; bigomega(1) returns 0 */
                if (minus > 0 && plus > 0
                    && bigomega((idx_t)minus) == 2
                    && bigomega((idx_t)plus)  == 2) {

                    /* allocate more space if needed */
                    if (cache_size == cache_cap) {
                        cache_cap = cache_cap ? cache_cap * 2 : 8;
                        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                    }
                    cache[cache_size++] = (seq_t)q;
                    last_q = q;
                    break;                 /* found the next term */
                }
            }
            ++q; /* try next candidate */
        }
    }

    return cache[n - 1];
}
