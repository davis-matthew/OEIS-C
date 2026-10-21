/*
 * A081070  Lucas(4n)-2, or 5*Fibonacci(2n)^2.
 * https://oeis.org/A081070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 8*a(n-2) + a(n-3), a(0) = 0, a(1) = 5, a(2) =
 *            45
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for already‑computed terms */
    static seq_t *cache = NULL;
    static idx_t cap    = 0;   /* allocated capacity */
    static idx_t size   = 0;   /* number of computed terms */

    /* Return base cases without touching the cache */
    if (n < 3) {
        switch (n) {
            case 0: return (seq_t)0;
            case 1: return (seq_t)5;
            case 2: return (seq_t)45;
        }
    }

    /* Lazy allocation and initialization of the cache */
    if (!cache) {
        cap = 8;                       /* start with a small capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* allocation failure – unlikely in the test harness */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)45;
        size = 3;                       /* terms 0..2 are now computed */
    }

    /* Expand the cache if needed */
    if (n >= size) {
        idx_t needed = n + 1;
        if (needed > cap) {
            size_t newcap = cap;
            while (newcap < needed) newcap <<= 1;  /* double until big enough */
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;             /* allocation failure */
            cache = tmp;
            cap = newcap;
        }

        /* Compute missing values using the recurrence */
        for (idx_t i = size; i <= n; ++i) {
            seq_t t1 = (seq_t)8 * cache[i-1];
            seq_t t2 = (seq_t)8 * cache[i-2];
            cache[i] = t1 - t2 + cache[i-3];
        }
        size = n + 1;
    }

    return cache[n];
}
