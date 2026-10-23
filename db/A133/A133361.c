/*
 * A133361  Multiply by 9 and reverse.
 * https://oeis.org/A133361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = reverse(9 * a(n-1)), a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A133361:  multiply by 9 and then reverse the decimal
 * digits.  The result is cached, so subsequent calls with the same or larger
 * indices are O(1) after the first computation.  The implementation works
 * uniformly for any integer type seq_t, including fixed‑width unsigned
 * integers and arbitrary‑precision integer classes (C++), because it only
 * uses the operators + – * / % and the helper function
 * “seq_t reverse(seq_t v)”. */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/reverse.h"

seq_t a(idx_t n)
{
    /* The cache stores a(1) … a(cap-1).  Elements are indexed
     * a(k) → cache[k-1].  Cache grows by doubling as needed. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity (elements)   */
    static size_t len   = 0;   /* number of terms currently stored */

    if (!cache) {
        /* Initialise cache for a(1) = 1 */
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;   /* a(1) */
        cap   = 1;
        len   = 1;
    }

    /* Grow the cache if it cannot hold the requested index */
    if ((size_t)n > cap) {
        size_t newcap = cap ? cap * 2 : 1;
        while (newcap < (size_t)n) newcap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = newcap;
    }

    /* Compute all missing values up to n */
    if ((size_t)n > len) {
        for (size_t i = len; i < (size_t)n; ++i) {
            seq_t prev = cache[i - 1];          /* a(i)   (since i starts at len) */
            seq_t prod = prev * (seq_t)9;       /* 9 * a(i) */
            cache[i]   = reverse(prod);         /* a(i+1) */
        }
        len = (size_t)n;
    }

    return cache[n - 1];
}
