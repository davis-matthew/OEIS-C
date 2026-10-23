/*
 * A031461  Numbers whose base-4 representation has the same number of 0's as
 *          3's.
 * https://oeis.org/A031461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A031461:
   a(n) = n-th positive integer whose base‑4 representation
   contains an equal number of 0's and 3's.  (OEIS offset 1)
   The implementation keeps a growing cache of the sequence
   so that each call runs in O(1) after the first few terms. */

#include <stdlib.h>
#include <stdbool.h>

static bool qualifies(seq_t x)
{
    unsigned long long cnt0 = 0, cnt3 = 0;   /* counters fit in 64 bits */
    seq_t t = x;
    while (t != (seq_t)0) {
        unsigned d = (unsigned)(t % (seq_t)4);
        if (d == 0) ++cnt0;
        else if (d == 3) ++cnt3;
        t /= (seq_t)4;
    }
    return cnt0 == cnt3;
}

seq_t a(idx_t n)
{
    /* The cache holds the sequence values up to the largest
       index requested so far.  It is populated on demand. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* current allocated capacity */
    static idx_t  count = 0;   /* number of values stored in cache */
    static seq_t  cur   = (seq_t)1;   /* next integer to test */

    /* Grow the cache to a reasonably large initial size. */
    if (cap == 0) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate sequence values until we have at least n of them. */
    while (count < n) {
        if (qualifies(cur)) {
            if (count == cap) { /* increase capacity */
                cap *= 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[count++] = cur;
        }
        ++cur;
    }

    return cache[n - 1];
}
