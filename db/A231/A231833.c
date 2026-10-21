/*
 * A231833  Number of n X 2 0..3 arrays with no element less than a strict
 *          majority of its horizontal and antidiagonal neighbors.
 * https://oeis.org/A231833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 34*a(n-2) + 86*a(n-3) + 91*a(n-4) + 46*a(n-5)
 *            + 11*a(n-6) + a(n-7) Initial values: a(1)=4, a(2)=50, a(3)=422,
 *            a(4)=3823, a(5)=34350, a(6)=308419, a(7)=2771101
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safety, though a(0) is undefined */
    static seq_t *cache = NULL;    /* memoization array */
    static idx_t cache_len = 0;    /* number of values stored */

    /* Allocate and initialise the first 7 terms on first use */
    if (cache == NULL) {
        cache_len = 7;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)50;
        cache[2] = (seq_t)422;
        cache[3] = (seq_t)3823;
        cache[4] = (seq_t)34350;
        cache[5] = (seq_t)308419;
        cache[6] = (seq_t)2771101;
    }

    /* Return stored value if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* Expand cache to hold values up to n */
    idx_t new_len = n;
    if (new_len < cache_len * 2)
        new_len = cache_len * 2;    /* grow by factor of 2 for efficiency */
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (tmp == NULL) return 0;      /* allocation failed – return 0 as fallback */
    cache = tmp;

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_len; i < n; ++i) {
        cache[i] = (seq_t)4 * cache[i - 1]
                 + (seq_t)34 * cache[i - 2]
                 + (seq_t)86 * cache[i - 3]
                 + (seq_t)91 * cache[i - 4]
                 + (seq_t)46 * cache[i - 5]
                 + (seq_t)11 * cache[i - 6]
                 + cache[i - 7];
    }

    cache_len = n;
    return cache[n - 1];
}
