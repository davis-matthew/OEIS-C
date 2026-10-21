/*
 * A225920  a(n) is the least k such that f(a(n-1)+1) + ... + f(k) >
 *          f(a(n-2)+1) + ... + f(a(n-1)) for n > 1, where f(n) = 1/(n+5) and
 *          a(1) = 1.
 * https://oeis.org/A225920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence defining a(n) a(1) = 1, a(2) = 13 for n>=3: a(n) =
 *            ceil( ((2*a(n-1)+11)^2 - 11*(2*a(n-2)+11)) / (2*(2*a(n-2)+11)) )
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated capacity in seq_t elements */
    static idx_t max_n = 0;  /* next index to compute (1‑based) */

    if (n == 0) return (seq_t)0;   /* undefined, for safety */

    /* Ensure the cache is large enough to hold n terms */
    if ((size_t)n > cap) {
        size_t new_cap = (size_t)n * 2 + 16;   /* growth factor + slack */
        void *tmp = realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();       /* out of memory */
        cache = (seq_t *)tmp;    /* explicit cast for C++ */
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    while (max_n < n) {
        ++max_n;  /* computing a(max_n) */

        seq_t val;
        if (max_n == 1) {
            val = (seq_t)1;
        } else if (max_n == 2) {
            val = (seq_t)13;
        } else {
            /* Use 128‑bit intermediate to avoid overflow on native types */
            __int128 tA = (__int128)((seq_t)(cache[max_n - 2] * (seq_t)2 + (seq_t)11));
            __int128 tB = (__int128)((seq_t)(cache[max_n - 3] * (seq_t)2 + (seq_t)11));

            __int128 num = tA * tA - (__int128)11 * tB;
            __int128 den = tB * 2;
            /* ceil division: (num + den - 1) / den   (den > 0) */
            __int128 res = (num + den - 1) / den;
            val = (seq_t)res;
        }
        cache[max_n - 1] = val;
    }

    return cache[n - 1];
}
