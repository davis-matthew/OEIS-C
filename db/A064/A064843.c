/*
 * A064843  a(n) = A064842(n)/2.
 * https://oeis.org/A064843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A064843 satisfies the linear recurrence
   a(1)=0, a(2)=1, a(3)=3, a(4)=9, a(5)=18
   a(n) = 3*a(n‑1) – 2*a(n‑2) – 2*a(n‑3) + 3*a(n‑4) – a(n‑5)  (n ≥ 6)  */

/* Include standard allocations header */
#include <stdlib.h>

/* External linkage function required by the test harness */
seq_t a(idx_t n)
{
    /* Static cache for memoisation – grows lazily */
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;

    if (n == 0) /* undefined for n=0, return 0 */
        return (seq_t)0;

    /* Allocate or extend the cache up to n */
    if (n > cached_n) {
        /* Allocate memory if not yet allocated */
        if (cache == NULL)
            cache = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        else
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));

        /* In case allocation failed – safety return (conceptually this
           should not happen in the test harness) */
        if (cache == NULL)
            return (seq_t)0;

        /* Initialise base cases if they are not yet present */
        if (cached_n < 5) {
            cache[1] = (seq_t)0;
            if (n >= 2) cache[2] = (seq_t)1;
            if (n >= 3) cache[3] = (seq_t)3;
            if (n >= 4) cache[4] = (seq_t)9;
            if (n >= 5) cache[5] = (seq_t)18;
        }

        /* Compute further terms using the recurrence */
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            if (i <= 5)  /* already set above */
                continue;

            /* Compute intermediate values to avoid unsigned underflow */
            seq_t t1 = (seq_t)3 * cache[i - 1];
            seq_t t4 = (seq_t)3 * cache[i - 4];
            seq_t sum = t1 + t4;

            seq_t t2 = (seq_t)2 * cache[i - 2];
            seq_t t3 = (seq_t)2 * cache[i - 3];
            seq_t t5 = cache[i - 5];

            seq_t sub = t2 + t3 + t5;
            cache[i] = sum - sub;   /* guaranteed non‑negative */
        }
        cached_n = n;
    }

    return cache[n];
}
