/*
 * A164445  Number of binary strings of length n with no substrings equal to
 *          0001 0010 or 0100.
 * https://oeis.org/A164445
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 40, u64 n <= 81, u128 n <= 163,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A164445: Number of binary strings of length n with no substrings equal to 0001, 0010 or 0100.
   Recurrence: a(n) = a(n-1) + a(n-2) + a(n-4) - a(n-6)  for n ≥ 10,
   with initial values a(4)=13, a(5)=22, a(6)=38, a(7)=66, a(8)=113, a(9)=193.  */
/* All computations are performed in the type seq_t. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n < 4) {
        return 0;           /* Undefined for n < 4; return 0 safely. */
    }

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;   /* Capacity of the cache array. */
    static idx_t  stored = 0;  /* Highest index currently computed. */

    /* Initialise cache on the first call. */
    if (cap == 0) {
        cap = 64;                          /* Initial capacity. */
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!cache) return 0;              /* Allocation failure. */
        for (idx_t i = 0; i < cap; ++i) cache[i] = 0;

        /* Set the known initial values. */
        cache[4] = (seq_t)13;
        cache[5] = (seq_t)22;
        cache[6] = (seq_t)38;
        cache[7] = (seq_t)66;
        cache[8] = (seq_t)113;
        cache[9] = (seq_t)193;
        stored = 9;
    }

    /* Grow the cache if necessary. */
    if (n >= cap) {
        idx_t new_cap = cap;
        while (new_cap <= n) new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!new_cache) return 0;          /* Allocation failure. */
        for (idx_t i = cap; i < new_cap; ++i) new_cache[i] = 0;
        cache = new_cache;
        cap   = new_cap;
    }

    /* Compute any values up to n that have not yet been computed. */
    if (stored < n) {
        for (idx_t i = stored + 1; i <= n; ++i) {
            if (i >= 10) {
                seq_t sum = cache[i - 1] + cache[i - 2];
                sum += cache[i - 4];
                sum -= cache[i - 6];
                cache[i] = sum;
            }
            /* For i < 10 the values are already initialised. */
        }
        stored = n;
    }

    return cache[n];
}
