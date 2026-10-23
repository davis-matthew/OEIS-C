/*
 * A044161  Numbers k such that string 3,2 occurs in the base 7 representation
 *          of k but not of k-1.
 * https://oeis.org/A044161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1445, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044161 : numbers k whose base‑7 representation contains the
   substring 32, but k‑1 does not.  The function returns the n‑th term
   (n >= 1).  It builds the terms lazily and keeps a static cache so that
   arbitrary ordering of calls is supported.  Operations are performed
   only with +, -, *, /, % and comparisons, so the code is valid for
   both C11 and C++17. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has32.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset starts at 1 */

    /* static cache for already computed terms */
    static int   initialized = 0;
    static seq_t *cache  = NULL;
    static size_t cache_used = 0;
    static size_t cache_cap   = 0;
    static seq_t last_checked = 0;

    if (!initialized) {
        cache_cap = 128;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_used = 0;
        initialized = 1;
    }

    /* return immediately if we already have the answer */
    if (n <= cache_used)
        return cache[n-1];

    /* generate terms until we reach the requested index */
    seq_t k = last_checked + 1;
    while (cache_used < n) {
        if (has32(k)) {
            if (!has32(k-1)) {
                if (cache_used >= cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_used++] = k;
            }
        }
        last_checked = k;
        ++k;
    }
    return cache[n-1];
}
