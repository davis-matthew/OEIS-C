/*
 * A122195  Numbers that are the sum of exactly 3 sets of Fibonacci numbers.
 * https://oeis.org/A122195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-4) + a(n-8) + 1 for n ≥ 10 Base values (1‑based
 *            index): a(1)=8, a(2)=11, a(3)=13, a(4)=14, a(5)=18, a(6)=19,
 *            a(7)=22, a(8)=23, a(9)=30 The sequence is generated using the
 *            recurrence, memoising values so that each term is computed only
 *            once, regardless of call order
 * Range:     u8 n <= 27, u16 n <= 72, u32 n <= 164, u64 n <= 349,
 *            u128 n <= 718, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* not used for the sequence */

    static seq_t *cache       = NULL;     /* 1‑based cache of computed terms */
    static idx_t   cap        = 0;        /* allocated size of cache  */
    static idx_t   max_comp   = 0;        /* largest index that has been computed */
    static bool    initialized = false;  /* lazy initialisation flag */

    /* Initialise the cache with the first nine terms. */
    if (!initialized) {
        cap = 10;                      /* indices 0 … 9 inclusive */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;           /* allocation failure – return 0 */

        /* Base values; cache[0] remains unused. */
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)11;
        cache[3] = (seq_t)13;
        cache[4] = (seq_t)14;
        cache[5] = (seq_t)18;
        cache[6] = (seq_t)19;
        cache[7] = (seq_t)22;
        cache[8] = (seq_t)23;
        cache[9] = (seq_t)30;

        max_comp = 9;                  /* highest term known so far */
        initialized = true;
    }

    /* Grow the cache if the requested index does not fit. */
    if (n + 1 > cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failure – return 0 */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute any missing terms up to n. */
    if (n > max_comp) {
        for (idx_t i = max_comp + 1; i <= n; ++i) {
            if (i >= 10) {
                /* Recurrence a(i) = a(i-4) + a(i-8) + 1. */
                cache[i] = cache[i - 4] + cache[i - 8] + (seq_t)1;
            } else {
                /* i ≤ 9 terms are already initialised. */
            }
        }
        max_comp = n;
    }

    return cache[n];
}
