/*
 * A232958  Number of 4Xn 0..3 arrays with no element x(i,j) adjacent to value
 *          3-x(i,j) horizontally, diagonally or antidiagonally, and top left
 *          element zero.
 * https://oeis.org/A232958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24*a(n-1) + 76*a(n-2) - 1008*a(n-3) - 992*a(n-4) +
 *            8762*a(n-5) - 7403*a(n-6) - 2174*a(n-7) + 3324*a(n-8) -
 *            416*a(n-9) - 96*a(n-10), with a(1)=64, a(2)=1029, a(3)=27441,
 *            a(4)=685251, a(5)=17429409, a(6)=442227825, a(7)=11228712351,
 *            a(8)=285082486341, a(9)=7238105184837, a(10)=183771148650939
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    /* 1‑based initial values (index 0 unused) */
    static const seq_t base[11] = {
        0,
        (seq_t)64,
        (seq_t)1029,
        (seq_t)27441,
        (seq_t)685251,
        (seq_t)17429409,
        (seq_t)442227825,
        (seq_t)11228712351,
        (seq_t)285082486341,
        (seq_t)7238105184837,
        (seq_t)183771148650939
    };

    if (n <= 10) return base[n];

    /* Static cache to allow arbitrary call order and reuse. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t max_cached = 0;

    /* Allocate or grow cache as needed. */
    if (cache == NULL) {
        cache_size = n + 1;
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed */
        for (idx_t i = 1; i <= 10; ++i) cache[i] = base[i];
        max_cached = 10;
    } else if ((size_t)(n + 1) > cache_size) {
        size_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cache_size = new_size;
    }

    /* If request is already cached, return it. */
    if (n <= max_cached) return cache[n];

    /* Compute new terms using the recurrence. */
    for (idx_t k = max_cached + 1; k <= n; ++k) {
        cache[k] =
            24 * cache[k - 1] + 76 * cache[k - 2]
          - 1008 * cache[k - 3] - 992 * cache[k - 4]
          + 8762 * cache[k - 5] - 7403 * cache[k - 6]
          - 2174 * cache[k - 7] + 3324 * cache[k - 8]
          - 416 * cache[k - 9] - 96 * cache[k - 10];
    }
    max_cached = n;
    return cache[n];
}
