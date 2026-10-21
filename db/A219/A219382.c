/*
 * A219382  Number of 2 X n arrays of the minimum value of corresponding
 *          elements and their horizontal, diagonal or antidiagonal neighbors
 *          in a random, but sorted with lexicographically nondecreasing rows
 *          and columns, 0..2 2 X n array.
 * https://oeis.org/A219382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5) for
 *            n > 10 Initial values (index starts at 1): a(1)=6, a(2)=7,
 *            a(3)=14, a(4)=25, a(5)=40, a(6)=66, a(7)=103, a(8)=157,
 *            a(9)=234, a(10)=341. The implementation memoizes computed values
 *            up to the largest n requested
 * Range:     u8 n <= 9, u16 n <= 36, u32 n <= 165, u64 n <= 165,
 *            u128 n <= 165, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for malloc, realloc, calloc */

static const seq_t init[11] = {
    0,    /* dummy for index 0 */
    6,    /* a(1) */
    7,    /* a(2) */
    14,   /* a(3) */
    25,   /* a(4) */
    40,   /* a(5) */
    66,   /* a(6) */
    103,  /* a(7) */
    157,  /* a(8) */
    234,  /* a(9) */
    341   /* a(10) */
};

seq_t a(idx_t n)
{
    /* Handle indices below the offset */
    if (n == 0)
        return 0;

    /* Memoization: cache holds values up to the largest n computed so far. */
    static seq_t *cache = NULL;     /* zero-initialized, will be allocated on first call */
    static idx_t cache_size = 0;    /* cache_size is the number of indices stored (max index + 1) */

    /* Initial allocation with the first 10 known terms. */
    if (cache_size == 0) {
        cache_size = 11;                          /* we need indices 0..10 */
        cache = (seq_t *)calloc((size_t)cache_size, sizeof(seq_t));
        if (!cache)
            return 0;  /* allocation failed: return 0 as a safe fallback */

        for (idx_t i = 1; i <= 10; ++i)
            cache[i] = init[i];
    }

    /* Return the value if already computed. */
    if (n < cache_size)
        return cache[n];

    /* Ensure the internal array is large enough to hold up to index n. */
    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        return 0;  /* allocation failed: return 0 as a safe fallback */

    cache = tmp;
    /* Fill new entries, using the recurrence. */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t termA = 5 * cache[i - 1];
        seq_t termB = 10 * cache[i - 3];
        seq_t termC = cache[i - 5];
        seq_t sum   = termA + termB + termC;

        seq_t termD = 10 * cache[i - 2];
        seq_t termE = 5 * cache[i - 4];
        seq_t sub   = termD + termE;

        cache[i] = sum - sub;   /* sum >= sub for all n >= 11 */
    }

    cache_size = new_size;
    return cache[n];
}
