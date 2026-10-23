/*
 * A033498  a(n) = a(n-1)+ a(round(2*(n-1)/3)) +a(round((n-1)/3)) starting
 *          a(1)=1.
 * https://oeis.org/A033498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 44, u32 n <= 221, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A033498: a(n) = a(n-1) + a(round(2*(n-1)/3)) + a(round((n-1)/3))
   with a(1) = 1.  A(0) is implicitly 1 to satisfy the recurrence. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static memoisation cache */
    static seq_t *cache = NULL;      /* array of computed values */
    static size_t capacity = 0;      /* allocated array size in elements */
    static idx_t  max_idx   = 0;     /* highest index already computed */

    if (n < 1)                     /* undefined for n < 1, return 0 */
        return (seq_t)0;

    /* Initialise cache on first call */
    if (cache == NULL) {
        capacity = 64;                 /* initial capacity */
        cache    = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;           /* allocation failure */
        /* Zero all entries; set base values */
        for (size_t i = 0; i < capacity; ++i)
            cache[i] = (seq_t)0;
        cache[0] = (seq_t)1;           /* implicit a(0) = 1 for recurrence */
        cache[1] = (seq_t)1;           /* a(1) = 1 */
        max_idx = 1;
    }

    /* Grow the cache if needed */
    if ((size_t)(n + 1) > capacity) {
        size_t new_cap = capacity;
        while ((size_t)(n + 1) > new_cap)
            new_cap <<= 1;             /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;           /* allocation failure */
        cache    = tmp;
        capacity = new_cap;
        /* Zero new slots beyond previous max_idx */
        for (size_t i = max_idx + 1; i < capacity; ++i)
            cache[i] = (seq_t)0;
    }

    /* Compute any missing values up to n */
    for (idx_t i = max_idx + 1; i <= n; ++i) {
        /* n == 1 is already handled separately; we compute for i >= 2 */
        idx_t t2 = (2 * i - 1) / 3;    /* round(2*(i-1)/3) */
        idx_t t3 = i / 3;              /* round((i-1)/3) */

        cache[i] = cache[i - 1] + cache[t2] + cache[t3];
    }

    max_idx = n;
    return cache[n];
}
