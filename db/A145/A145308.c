/*
 * A145308  Numbers n such that there exists x in N : (x+1)^3-x^3=79*n^2.
 * https://oeis.org/A145308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A145308
   a(n) : numbers n such that 3x^2+3x+1 = 79*n^2 for some integer x.
   The sequence satisfies the linear recurrence:
        a(1) = 19,
        a(2) = 8669719,
        a(n+2) = 456302*a(n+1) - a(n)   for n ≥ 1.
   The recurrence is used to compute a(n) efficiently.
   The function caches computed terms so that subsequent calls are O(1)
   after the needed terms have been generated. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Handle trivial cases */
    if (n == 0) return 0;                 /* undefined below offset – return 0 */
    if (n == 1) return 19;
    if (n == 2) return 8669719;

    /* Static cache: cache[0] = a(1), cache[1] = a(2), … */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;          /* number of terms stored */
    static size_t cache_cap = 0;          /* allocated slots */

    /* Initialize cache with the first two terms if needed */
    if (cache_len == 0) {
        cache_cap = 4;                    /* arbitrary initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;             /* allocation failure – undefined */
        cache[0] = 19;
        cache[1] = 8669719;
        cache_len = 2;
    }

    /* If requested term already cached, return it */
    if (n <= cache_len) return cache[n-1];

    /* Extend cache up to n */
    for (size_t i = cache_len + 1; i <= n; ++i) {
        /* Ensure enough capacity */
        if (cache_len + 1 > cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;          /* allocation failure – undefined */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* a(i) = 456302 * a(i-1) - a(i-2) */
        seq_t next = (seq_t)456302 * cache[cache_len-1] - cache[cache_len-2];
        cache[cache_len] = next;
        ++cache_len;
    }

    return cache[n-1];
}
