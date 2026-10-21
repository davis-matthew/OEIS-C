/*
 * A234762  Number of (n+1) X (1+1) 0..3 arrays with 2 X 2 edge jumps all no
 *          more than +1 in one of the clockwise or counterclockwise
 *          directions but not both.
 * https://oeis.org/A234762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 14*a(n-2) - a(n-3) - 25*a(n-4) - 10*a(n-5), with
 *            a(1)=56, a(2)=240, a(3)=916, a(4)=3800, a(5)=14828
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

/* Computes A234762(n).  External linkage. */
seq_t a(idx_t n)
{
    /* Initial values for n = 1..5 */
    static const seq_t INIT[5] = {56, 240, 916, 3800, 14828};

    /* Static cache of values computed so far */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;  /* number of terms stored in cache */

    /* Lazy initialization of the cache with the first five terms */
    if (cache == NULL) {
        cache_size = 5;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_size);
        assert(cache);  /* abort on allocation failure */
        for (idx_t i = 0; i < 5; ++i)
            cache[(size_t)i] = INIT[i];
    }

    /* If the requested term is already in the cache, return it */
    if (n <= cache_size || n == 0) {
        /* n==0 is undefined; return 0 for safety */
        if (n == 0) return (seq_t)0;
        return cache[(size_t)(n - 1)];
    }

    /* Grow the cache to accommodate the requested term */
    size_t new_size = (size_t)n;
    seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    assert(new_cache);  /* abort on allocation failure */
    cache = new_cache;

    /* Compute terms from cache_size+1 up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t val =
            cache[(size_t)(i - 2)]                  /* a(i-1) */
          + 14 * cache[(size_t)(i - 3)]              /* 14*a(i-2) */
          - cache[(size_t)(i - 4)]                  /* a(i-3) */
          - 25 * cache[(size_t)(i - 5)]              /* 25*a(i-4) */
          - 10 * cache[(size_t)(i - 6)];             /* 10*a(i-5) */
        cache[(size_t)(i - 1)] = val;
    }
    cache_size = n;

    return cache[(size_t)(n - 1)];
}
