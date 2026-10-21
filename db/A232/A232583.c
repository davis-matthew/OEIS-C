/*
 * A232583  Number of (n+1)X(2+1) 0..2 arrays with every element next to
 *          itself plus and minus one within the range 0..2 horizontally or
 *          antidiagonally, with no adjacent elements equal.
 * https://oeis.org/A232583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=10, a(2)=34, a(3)=124, a(4)=456 for n≥5: a(n)=4*a(n-1) -
 *            a(n-2) - a(n-3) + 2*a(n-4)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* Undefined in OEIS – avoid UB */

    static seq_t *cache = NULL;           /* Cached terms, 1‑based index (cache[0]=a(1)) */
    static idx_t cached_n = 0;            /* Number of terms stored */

    /* Initialise the first four terms once */
    if (cache == NULL) {
        cached_n = 4;
#ifdef __cplusplus
        cache = new seq_t[4];
#else
        cache = (seq_t*)malloc(4 * sizeof(seq_t));
#endif
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)10;   /* a(1) */
        cache[1] = (seq_t)34;   /* a(2) */
        cache[2] = (seq_t)124;  /* a(3) */
        cache[3] = (seq_t)456;  /* a(4) */
    }

    /* Return a cached value if possible */
    if (n <= cached_n) {
        return cache[n - 1];
    }

    /* Grow the cache to hold up to n terms */
    idx_t new_size = n;
#ifdef __cplusplus
    seq_t *new_cache = new seq_t[new_size];
    for (size_t i = 0; i < cached_n; ++i) new_cache[i] = cache[i];
    delete[] cache;
    cache = new_cache;
#else
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
#endif

    /* Compute missing terms using the recurrence */
    for (idx_t i = cached_n; i < new_size; ++i) {
        seq_t v = cache[i - 1] * (seq_t)4;
        v -= cache[i - 2];
        v -= cache[i - 3];
        v += cache[i - 4] * (seq_t)2;
        cache[i] = v;
    }

    cached_n = new_size;
    return cache[n - 1];
}
