/*
 * A195806  Number of triangular of a 5 X 5 X 5 0..n arrays with all rows and
 *          diagonals having the same length having the same sum, with corners
 *          zero.
 * https://oeis.org/A195806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows the linear recurrence a(n) = a(n-14) - 3*a(n-13) +
 *            2*a(n-12) - a(n-11) + 6*a(n-10) - 5*a(n-9) - 3*a(n-8) + 3*a(n-6)
 *            + 5*a(n-5) - 6*a(n-4) + a(n-3) - 2*a(n-2) + 3*a(n-1) with
 *            initial terms a(1)=16, a(2)=105, a(3)=496, a(4)=1759, a(5)=5052,
 *            a(6)=12469, a(7)=27412, a(8)=55059, a(9)=1029...
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity (in elements) */
    static size_t len   = 0;   /* number of values stored (cache[0] = a(1)) */

    /* Initialisation with known first 14 terms */
    if (cache == NULL) {
        /* Allocate a small initial capacity */
        cap = 64;
        cache = (seq_t*)malloc(cap * sizeof(*cache));
        static const seq_t init[14] = {
            (seq_t)16ULL,          (seq_t)105ULL,          (seq_t)496ULL,
            (seq_t)1759ULL,        (seq_t)5052ULL,         (seq_t)12469ULL,
            (seq_t)27412ULL,       (seq_t)55059ULL,        (seq_t)102952ULL,
            (seq_t)181543ULL,      (seq_t)304908ULL,       (seq_t)491563ULL,
            (seq_t)765184ULL,      (seq_t)1155567ULL
        };
        memcpy(cache, init, sizeof(init));
        len = 14;
    }

    /* Return previously computed value if available */
    if (n <= len)
        return cache[n - 1];

    /* Grow the cache to hold at least n terms */
    if (cap < (size_t)n) {
        size_t newcap = cap ? cap : 8;
        while (newcap < (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t*)malloc(newcap * sizeof(*tmp));
        memcpy(tmp, cache, len * sizeof(*tmp));
        free(cache);
        cache = tmp;
        cap   = newcap;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = len + 1; i <= n; ++i) {
        /* i corresponds to a(i) with 1‑based index.
           The array is 0‑based: cache[0] = a(1).
           Thus a(m) is stored at cache[m-1]. */
        seq_t v = cache[i - 15];                  /* a(i-14) */
        v -= (seq_t)3 * cache[i - 14];            /* a(i-13) */
        v += (seq_t)2 * cache[i - 13];            /* a(i-12) */
        v -=                cache[i - 12];        /* a(i-11) */
        v += (seq_t)6 * cache[i - 11];            /* a(i-10) */
        v -= (seq_t)5 * cache[i - 10];            /* a(i- 9) */
        v -= (seq_t)3 * cache[i -  9];            /* a(i- 8) */
        v += (seq_t)3 * cache[i -  7];            /* a(i- 6) */
        v += (seq_t)5 * cache[i -  6];            /* a(i- 5) */
        v -= (seq_t)6 * cache[i -  5];            /* a(i- 4) */
        v +=                cache[i -  4];        /* a(i- 3) */
        v -= (seq_t)2 * cache[i -  3];            /* a(i- 2) */
        v += (seq_t)3 * cache[i -  2];            /* a(i- 1) */
        cache[i - 1] = v;                          /* store a(i) */
    }
    len = n;
    return cache[n - 1];
}
