/*
 * A238173  Number of (n+1) X (1+1) 0..2 arrays with the maximum plus the
 *          minimum of every 2 X 2 subblock equal.
 * https://oeis.org/A238173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 10*a(n-1) - 22*a(n-2) - 20*a(n-3) + 63*a(n-4) - 6*a(n-5) - 24*a(n-6)
 * Initial terms: a(1)=81, a(2)=423, a(3)=2329, a(4)=13447, a(5)=79641, a(6)=479703
 */
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset is 1; behaviour for n<1 is irrelevant */

    static seq_t *cache = NULL;   /* cache[i] holds a(i+1) */
    static size_t len   = 0;      /* number of terms currently stored in cache */
    static size_t cap   = 0;      /* allocated capacity of cache */

    /* first 6 terms of the sequence */
    static const seq_t base[6] = {81, 423, 2329, 13447, 79641, 479703};

    /* initialise cache on the first call */
    if (!cache) {
        size_t init_cap = n > 16 ? n : 16;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) abort();      /* fatal if allocation fails */
        for (size_t i = 0; i < 6; ++i) cache[i] = base[i];
        len = 6;
        cap = init_cap;
    }

    /* if more terms are required, grow the cache and compute them */
    if ((idx_t)len < n) {
        /* enlarge capacity if necessary */
        if ((idx_t)cap < n) {
            size_t new_cap = cap * 2;
            if (new_cap < (size_t)n) new_cap = n;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cap   = new_cap;
        }
        /* compute terms from len+1 up to n (inclusive) */
        for (idx_t i = len + 1; i <= n; ++i) {
            seq_t val = 10 * cache[i - 2]          /* a(i-1)   */
                      - 22 * cache[i - 3]          /* a(i-2)   */
                      - 20 * cache[i - 4]          /* a(i-3)   */
                      + 63 * cache[i - 5]          /* a(i-4)   */
                      - 6  * cache[i - 6]          /* a(i-5)   */
                      - 24 * cache[i - 7];         /* a(i-6)   */
            cache[i - 1] = val;                   /* store a(i) */
        }
        len = n;
    }

    return cache[n - 1];
}
