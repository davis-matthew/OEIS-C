/*
 * A196984  Number of n X 3 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 2,3,4,1,0
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 6*a(n-3) + 11*a(n-4) + 14*a(n-5) +
 *            4*a(n-6) - 4*a(n-7) - 4*a(n-8) - a(n-9) Initial values: a(1)=1,
 *            a(2)=3, a(3)=10, a(4)=25, a(5)=81, a(6)=242, a(7)=707,
 *            a(8)=2095, a(9)=6224
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;  /* number of terms currently stored */
    static size_t cache_cap  = 0;  /* allocated capacity */

    /* base values a(1)..a(9) */
    static const seq_t base[9] = {
        1, 3, 10, 25, 81, 242, 707, 2095, 6224
    };

    /* initialize cache on first call */
    if (cache == NULL) {
        cache_cap = 9;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) return 0;  /* allocation failure (not expected) */
        for (size_t i = 0; i < cache_cap; ++i)
            cache[i] = base[i];
        cache_size = cache_cap;
    }

    if (n == 0) return 0; /* undefined for n<1, but return 0 as safe default */

    /* if requested term already computed, return it */
    if (n <= cache_size)
        return cache[n - 1];

    /* compute terms up to n */
    for (size_t i = cache_size + 1; i <= (size_t)n; ++i) {
        /* ensure capacity */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return 0; /* allocation failure (not expected) */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* indices: a(k) stored at cache[k-1] */
        seq_t val = cache[i - 2];                                 /* a(i-1) */
        val += 2 * cache[i - 3];                                  /* 2*a(i-2) */
        val += 6 * cache[i - 4];                                  /* 6*a(i-3) */
        val += 11 * cache[i - 5];                                 /* 11*a(i-4) */
        val += 14 * cache[i - 6];                                 /* 14*a(i-5) */
        val += 4 * cache[i - 7];                                  /* 4*a(i-6) */
        val -= 4 * cache[i - 8];                                  /* -4*a(i-7) */
        val -= 4 * cache[i - 9];                                  /* -4*a(i-8) */
        val -= cache[i - 10];                                     /* -a(i-9) */

        cache[cache_size++] = val;
    }

    return cache[n - 1];
}
