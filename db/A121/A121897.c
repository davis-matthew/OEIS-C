/*
 * A121897  a(n) = 4*a(n-1)*a(n-2)*a(n-3)*a(n-4) - a(n-5).
 * https://oeis.org/A121897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1)*a(n-2)*a(n-3)*a(n-4) - a(n-5) with offset 1 and
 *            initial values a(1)=a(2)=a(3)=a(4)=a(5)=1
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 10, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
static inline seq_t* alloc_array(size_t n) { return new seq_t[n]; }
static inline void free_array(seq_t *p)      { delete[] p; }
#else
static inline seq_t* alloc_array(size_t n) { return (seq_t*)malloc(sizeof(seq_t)*n); }
static inline void free_array(seq_t *p)      { free(p); }
#endif

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;       /* current allocation size – 1 for unused zero index */
    static idx_t last = 0;       /* highest index actually cached */

    if (n <= 5) return (seq_t)1;

    /* initialise cache if first call */
    if (!cache) {
        cap = 16;
        cache = alloc_array(cap + 1);          /* +1 so indices start at 1 */
        if (!cache) return (seq_t)0;           /* allocation failed – abort quietly */
        for (idx_t i = 1; i <= 5; ++i) cache[i] = (seq_t)1;
        last = 5;
    }

    /* grow cache if needed */
    if ((size_t)n > cap) {
        size_t new_cap = cap;
        while ((size_t)n > new_cap) new_cap <<= 1;
        seq_t *new_cache = alloc_array(new_cap + 1);
        if (!new_cache) return (seq_t)0;
        for (size_t i = 0; i <= cap; ++i) new_cache[i] = cache[i];
        free_array(cache);
        cache = new_cache;
        cap   = new_cap;
    }

    /* compute new values up to requested n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t prod = (seq_t)4;
        prod *= cache[i-1];
        prod *= cache[i-2];
        prod *= cache[i-3];
        prod *= cache[i-4];
        cache[i] = prod - cache[i-5];
    }
    last = n;
    return cache[n];
}
