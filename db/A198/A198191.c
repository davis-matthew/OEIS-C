/*
 * A198191  Start with sequence 1,2,3, after this a(n) is smallest number >
 *          a(n-1) coprime to sum of previous elements.
 * https://oeis.org/A198191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 472, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A198191.
   Recurrence: a(1)=1, a(2)=2, a(3)=3.
   For n>3, a(n) is the smallest integer > a(n‑1) that is coprime to
   S(n‑1) = a(1)+…+a(n‑1).  The function memoises results so that
   any call order is supported.  It uses a dynamic array for the
   sequence and a running sum.  GCD is obtained from ../utils/gcd.h. */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* Static cache for computed terms and their cumulative sum. */
    static seq_t *cache = NULL;   /* sequence values */
    static size_t len = 0;        /* number of terms stored */
    static size_t cap = 0;        /* capacity of the cache */
    static seq_t sum = 0;         /* sum of stored terms */

    /* Initialise cache with first three terms on first call. */
    if (len == 0) {
        cap = 64;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        /* a(1) = 1, a(2) = 2, a(3) = 3 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        len = 3;
        sum = (seq_t)6; /* 1 + 2 + 3 */
    }

    /* If requested index already computed, return it. */
    if (n <= len)
        return cache[n - 1];

    /* Compute missing terms up to n. */
    for (idx_t i = len + 1; i <= n; ++i) {
        seq_t last = cache[len - 1];
        seq_t candidate = last + (seq_t)1;

        /* Find smallest candidate > last that is coprime to current sum. */
        while (gcd(candidate, sum) != (seq_t)1)
            ++candidate;

        /* Grow cache if necessary. */
        if (len == cap) {
            cap <<= 1;  /* double capacity */
            cache = (seq_t*)realloc(cache, cap * sizeof(seq_t));
        }

        cache[len] = candidate;
        sum += candidate;
        ++len;
    }

    return cache[n - 1];
}
