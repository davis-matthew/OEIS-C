/*
 * A184543  Number of (n+2) X 6 binary arrays with each 3 X 3 subblock having
 *          rows and columns in lexicographically nondecreasing order.
 * https://oeis.org/A184543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=220, a(2)=537, a(3)=1048, a(4)=1837, a(5)=3024, a(6)=4774,
 *            a(7)=7307
 *            a(n)=7*a(n-1)-21*a(n-2)+35*a(n-3)-35*a(n-4)+21*a(n-5)-7*a(n-6)+a(n-7)
 *            (n>7) Lazy memoisation is used so that calls in arbitrary order
 *            remain O(n) total
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 115, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoised terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;

    /* Initialise cache on first call */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0; /* Allocation failed; return zero as fallback */
        cache[0] = 220;   /* a(1) */
        cache[1] = 537;   /* a(2) */
        cache[2] = 1048;  /* a(3) */
        cache[3] = 1837;  /* a(4) */
        cache[4] = 3024;  /* a(5) */
        cache[5] = 4774;  /* a(6) */
        cache[6] = 7307;  /* a(7) */
        len = 7;
    }

    /* Return cached value if possible */
    if (n <= len) return cache[n - 1];

    /* Grow cache capacity if needed */
    while (cap < (size_t)n) {
        size_t newcap = cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0; /* Allocation failed; return zero as fallback */
        cache = tmp;
        cap = newcap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = (idx_t)(len + 1); i <= n; ++i) {
        /* Indices: cache[0] holds a(1) */
        seq_t t1 = 7 * cache[i - 2];          /* a(i-1) */
        seq_t t2 = 21 * cache[i - 3];         /* a(i-2) */
        seq_t t3 = 35 * cache[i - 4];         /* a(i-3) */
        seq_t t4 = 35 * cache[i - 5];         /* a(i-4) */
        seq_t t5 = 21 * cache[i - 6];         /* a(i-5) */
        seq_t t6 = 7 * cache[i - 7];          /* a(i-6) */
        seq_t sum1 = t1 + t3 + t5 + cache[i - 8]; /* a(i-7) included */
        seq_t sum2 = t2 + t4 + t6;
        cache[i - 1] = sum1 - sum2;           /* a(i) */
    }

    len = n;
    return cache[n - 1];
}
