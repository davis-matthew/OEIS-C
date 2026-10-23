/*
 * A126939  "Model 1" for number of free alkanes on n points.
 * https://oeis.org/A126939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 7*a(n-3) + 3*a(n-4) + 3*a(n-5) +
 *            a(n-6) (n ≥ 6), with initial values: a(0)=1, a(1)=1, a(2)=3,
 *            a(3)=11, a(4)=35, a(5)=107
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t size = 0;          /* current number of cached terms */

    /* Initialize the cache with the six known terms. */
    if (size == 0) {
        cache = (seq_t *)malloc(6 * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure */

        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)1;   /* a(1) */
        cache[2] = (seq_t)3;   /* a(2) */
        cache[3] = (seq_t)11;  /* a(3) */
        cache[4] = (seq_t)35;  /* a(4) */
        cache[5] = (seq_t)107; /* a(5) */

        size = 6;
        if (n < 6) return cache[n];
    }

    /* Return cached value if already computed. */
    if (n < size) return cache[n];

    /* Grow the cache to hold terms up to n. */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) return (seq_t)0;   /* allocation failure */
    cache = tmp;

    /* Compute new terms using the recurrence. */
    for (idx_t i = size; i <= n; ++i) {
        seq_t val = (seq_t)2 * cache[i - 1];
        val += cache[i - 2];
        val += (seq_t)7 * cache[i - 3];
        val += (seq_t)3 * cache[i - 4];
        val += (seq_t)3 * cache[i - 5];
        val += cache[i - 6];
        cache[i] = val;
    }

    size = n + 1;
    return cache[n];
}
