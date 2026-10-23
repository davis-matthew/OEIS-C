/*
 * A166929  Positive integers m such that m^4 = a^2 + b^2 and a + b = c^2 for
 *          some coprime integers a, b, c.
 * https://oeis.org/A166929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1764*a(n-1)*a(n-3) - 3107*a(n-2)^2) / a(n-4) with
 *            a(1)=1, a(2)=13, a(3)=1525, a(4)=2165017
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* memoisation cache for out‑of‑order queries */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements stored */

    if (n == 0) return 0;          /* undefined term, guard */

    /* return cached value if already computed */
    if ((size_t)n <= cache_len) {
        return cache[n - 1];
    }

    /* extend cache up to requested n */
    size_t new_len = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return 0;            /* allocation failed: safe fallback */
    cache = tmp;

    /* compute missing terms */
    for (size_t i = cache_len + 1; i <= new_len; ++i) {
        seq_t val;
        if (i == 1) {
            val = (seq_t)1;
        } else if (i == 2) {
            val = (seq_t)13;
        } else if (i == 3) {
            val = (seq_t)1525;
        } else if (i == 4) {
            val = (seq_t)2165017;
        } else {
            seq_t a1 = cache[i - 2];   /* a(i-1)  */
            seq_t a2 = cache[i - 3];   /* a(i-2)  */
            seq_t a3 = cache[i - 4];   /* a(i-3)  */
            seq_t a4 = cache[i - 5];   /* a(i-4)  */

            /* numerator = 1764*a1*a3 - 3107*a2*a2 */
            seq_t part1 = (seq_t)1764 * a1 * a3;
            seq_t part2 = (seq_t)3107 * a2 * a2;
            seq_t numerator = part1 - part2;

            val = numerator / a4;      /* division is exact */
        }
        cache[i - 1] = val;
    }

    cache_len = new_len;
    return cache[n - 1];
}
