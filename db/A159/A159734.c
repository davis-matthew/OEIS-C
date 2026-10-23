/*
 * A159734  Number of permutations of 5 indistinguishable copies of 1..n
 *          arranged in a circle with exactly 2 local maxima.
 * https://oeis.org/A159734
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 130*a(n-1) - 5260*a(n-2) + 68760*a(n-3) * - 362880*a(n-4)
 *            + 677376*a(n-5) (for n>6) * Initial values: a(2)=80, a(3)=8520,
 *            a(4)=659560, * a(5)=46412200, a(6)=3121135440
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 2)
        return 0;                 /* outside the offset, value irrelevant */

    /* initial terms a(2)..a(6) */
    static const seq_t init[5] = {
        (seq_t)80ULL,                /* a(2) */
        (seq_t)8520ULL,              /* a(3) */
        (seq_t)659560ULL,            /* a(4) */
        (seq_t)46412200ULL,          /* a(5) */
        (seq_t)3121135440ULL         /* a(6) */
    };

    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;     /* number of terms stored (a(2)..a(last)) */
    static size_t cache_cap = 0;     /* allocated capacity */

    /* ensure the cache is initialised */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 5; ++i)
            cache[i] = init[i];
        cache_len = 5;              /* a(2)..a(6) stored */
    }

    /* fast return for n within cache */
    if (n <= 6)
        return cache[n - 2];

    /* need to compute up to n, extend cache if necessary */
    size_t last_n = cache_len + 1;           /* highest n stored so far */
    if (n > last_n) {
        size_t needed = n - 1;               /* need indices 0 .. n-2 inclusive */
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < needed)
                new_cap *= 2;
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* compute terms from last_n+1 up to n */
        for (size_t k = last_n + 1; k <= n; ++k) {
            size_t idx = k - 2;      /* cache index for a(k) */

            seq_t val = (seq_t)130 * cache[idx - 1];
            val -= (seq_t)5260 * cache[idx - 2];
            val += (seq_t)68760 * cache[idx - 3];
            val -= (seq_t)362880 * cache[idx - 4];
            val += (seq_t)677376 * cache[idx - 5];

            cache[idx] = val;
        }
        cache_len = n - 1;
    }

    return cache[n - 2];
}
