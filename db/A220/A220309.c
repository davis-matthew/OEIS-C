/*
 * A220309  Majority value maps: number of n X 2 binary arrays indicating the
 *          locations of corresponding elements equal to at least half of
 *          their horizontal, vertical and antidiagonal neighbors in a random
 *          0..3 n X 2 array.
 * https://oeis.org/A220309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 2*a(n-2) - 8*a(n-3) - 3*a(n-4) + 9*a(n-5) -
 *            2*a(n-6) - 2*a(n-7) - a(n-8) + a(n-9) for n > 10 Initial values
 *            (1‑indexed): a(1)=2, a(2)=9, a(3)=40, a(4)=155, a(5)=617,
 *            a(6)=2457, a(7)=9785, a(8)=38986, a(9)=155300, a(10)=618678
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_10[10] = {
    (seq_t)2, (seq_t)9, (seq_t)40, (seq_t)155, (seq_t)617,
    (seq_t)2457, (seq_t)9785, (seq_t)38986, (seq_t)155300, (seq_t)618678
};

static seq_t *cache = NULL;     /* 1‑indexed cache of computed terms   */
static idx_t  cache_len = 0;    /* Highest index stored so far          */
static size_t cache_cap = 0;    /* Current allocated capacity           */

/* Allocate or enlarge the cache to hold indices up to n (1‑based). */
static void cache_ensure(idx_t n) {
    if (n <= cache_len) return;

    size_t new_cap = (size_t)n + 1;      /* include index 0 (unused) */

    /* Allocate new array */
#if defined(__cplusplus)
    seq_t *new_cache = new seq_t[new_cap];
#else
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif

    /* Copy existing data, if any */
    if (cache) {
        for (idx_t i = 1; i <= cache_len; ++i) new_cache[i] = cache[i];
#if defined(__cplusplus)
        delete[] cache;
#else
        free(cache);
#endif
    }

    cache = new_cache;
    cache_cap = new_cap;

    /* Fill missing entries */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 10) {
            cache[i] = init_10[i - 1];
        } else {
            cache[i] = (seq_t)4 * cache[i - 1]
                     + (seq_t)2 * cache[i - 2]
                     - (seq_t)8 * cache[i - 3]
                     - (seq_t)3 * cache[i - 4]
                     + (seq_t)9 * cache[i - 5]
                     - (seq_t)2 * cache[i - 6]
                     - (seq_t)2 * cache[i - 7]
                     - cache[i - 8]
                     + cache[i - 9];
        }
    }

    cache_len = n;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* offset 1; a(0) unused */
    cache_ensure(n);
    return cache[n];
}
