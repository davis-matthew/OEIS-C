/*
 * A127620  Number of walks from (0,0) to (n,n) in the region 0 <= x-y <= 6
 *          with the steps (1,0), (0, 1), (2,0) and (0,2).
 * https://oeis.org/A127620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) is defined by the generating function (1 - 5x - 6x^2 +
 *            11x^3 + 12x^4 - 4x^5)/(1 - 6x - 5x^2 + 24x^3 + 28x^4 + 6x^5 -
 *            8x^6). The resulting linear recurrence of order 6 is: a(n) = 6
 *            a(n-1) + 5 a(n-2) - 24 a(n-3) - 28 a(n-4) - 6 a(n-5) + 8 a(n-6),
 *            with initial values a(0)=1, a(1)=1, a(2)=5, a(3...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /*--------------------------------------------------------------*/
    /* 1.  Static cache – type depends on compilation target.     */
    /*--------------------------------------------------------------*/
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    /* initialise the first six values only once */
    if (cache.empty()) {
        cache.reserve(6);
        cache.push_back((seq_t)1); /* a(0) */
        cache.push_back((seq_t)1); /* a(1) */
        cache.push_back((seq_t)5); /* a(2) */
        cache.push_back((seq_t)22);/* a(3) */
        cache.push_back((seq_t)117);/* a(4) */
        cache.push_back((seq_t)654);/* a(5) */
    }
    /* Grow the cache if needed */
    if ((idx_t)cache.size() <= n) {
        cache.resize((size_t)n + 1);
        /* compute missing terms */
        for (size_t i = 6; i <= (size_t)n; ++i) {
            seq_t v1 = cache[i-1];
            seq_t v2 = cache[i-2];
            seq_t v3 = cache[i-3];
            seq_t v4 = cache[i-4];
            seq_t v5 = cache[i-5];
            seq_t v6 = cache[i-6];
            cache[i] = (seq_t)( (seq_t)6 * v1 + (seq_t)5 * v2
                                - (seq_t)24 * v3 - (seq_t)28 * v4
                                - (seq_t)6 * v5 + (seq_t)8 * v6 );
        }
    }
    return cache[(size_t)n];
#else
    /* C implementation using malloc/realloc */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;      /* number of stored elements */

    /* initialise first six values once */
    if (cache_size == 0) {
        cache_size = 6;                        /* indices 0..5 */
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;                  /* unlikely in test env */
        cache[0] = (seq_t)1; /* a(0) */
        cache[1] = (seq_t)1; /* a(1) */
        cache[2] = (seq_t)5; /* a(2) */
        cache[3] = (seq_t)22;/* a(3) */
        cache[4] = (seq_t)117;/* a(4) */
        cache[5] = (seq_t)654;/* a(5) */
    }

    /* extend the cache if necessary */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;    /* allocation failure – unlikely */
        cache = tmp;
        /* compute missing terms from cache_size up to n */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            seq_t v1 = cache[i-1];
            seq_t v2 = cache[i-2];
            seq_t v3 = cache[i-3];
            seq_t v4 = cache[i-4];
            seq_t v5 = cache[i-5];
            seq_t v6 = cache[i-6];
            cache[i] = (seq_t)( (seq_t)6 * v1 + (seq_t)5 * v2
                                - (seq_t)24 * v3 - (seq_t)28 * v4
                                - (seq_t)6 * v5 + (seq_t)8 * v6 );
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
#endif
}
