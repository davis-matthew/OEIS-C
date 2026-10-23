/*
 * A130120  a(0)=1. a(n) = number of earlier terms of the sequence that divide
 *          n(n+1)/2.
 * https://oeis.org/A130120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A130120: a(0)=1, a(n) = # { k < n | a(k) divides n*(n+1)/2 } */
/* Computes values on demand, memoising previous terms up to the maximum
   index requested.  The implementation runs in O(n²) time for the largest
   n requested, which is comfortably fast for n ≈ 10 000. */
#include <stdlib.h>

static seq_t *cache   = NULL;  /* memoised sequence values */
static idx_t  size    = 0;     /* number of values stored (>=1 after init) */
static size_t cap     = 0;     /* allocated capacity */

seq_t a(idx_t n)
{
    /* initialise cache with a(0)=1 */
    if (size == 0) {
        cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!cache) abort();          /* out of memory */
        cache[0] = 1;
        size    = 1;
    }

    /* compute values up to index n if needed */
    while (size <= n) {
        /* ensure enough space */
        if (size >= cap) {
            size_t new_cap = cap * 2 + 1;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            if (!tmp) abort();          /* out of memory */
            cache = tmp;
            cap   = new_cap;
        }

        idx_t   cur = size;                             /* index being computed */
        seq_t   T   = (seq_t)cur * (((seq_t)cur) + 1) / 2; /* n*(n+1)/2 */

        seq_t   cnt = 0;
        for (idx_t i = 0; i < size; ++i)
            if (T % cache[i] == 0)
                ++cnt;

        cache[size++] = cnt;
    }

    return cache[n];
}
