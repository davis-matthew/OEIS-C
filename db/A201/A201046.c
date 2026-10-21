/*
 * A201046  Number of -n..n arrays of 7 elements with adjacent element
 *          differences also in -n..n.
 * https://oeis.org/A201046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 14, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A201046: Number of -n..n arrays of 7 elements with adjacent differences also in -n..n.
   Recurrence (valid for n > 8, offset is 1):
   a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4)
          + 56*a(n-5) - 28*a(n-6) + 8*a(n-7) - a(n-8).
   Initial values: a(1)=577, a(2)=17981, a(3)=182349, a(4)=1042167,
                   a(5)=4211559, a(6)=13497523, a(7)=36644243, a(8)=87831605. */

#include <stdlib.h>

/* Unique external linkage */
seq_t a(idx_t n)
{
    /* Static cache for memoisation; index 0 is unused */
    static seq_t *cache = NULL;
    static size_t cached_n = 0;   /* highest n that has been computed */

    if (n == 0) return 0;         /* undefined below offset, return 0 as placeholder */

    /* Initialise cache once with the first 8 terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 9);  /* indices 0..8 */
        if (cache == NULL) abort(); /* memory failure */
        cache[1] = (seq_t)577;
        cache[2] = (seq_t)17981;
        cache[3] = (seq_t)182349;
        cache[4] = (seq_t)1042167;
        cache[5] = (seq_t)4211559;
        cache[6] = (seq_t)13497523;
        cache[7] = (seq_t)36644243;
        cache[8] = (seq_t)87831605;
        cached_n = 8;
    }

    /* If already computed, return stored value */
    if ((size_t)n <= cached_n) {
        return cache[(size_t)n];
    }

    /* Grow cache to accommodate up to n */
    size_t new_cap = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (new_cap + 1));
    if (tmp == NULL) abort();      /* memory failure */
    cache = tmp;

    /* Compute missing terms using the recurrence */
    for (size_t i = cached_n + 1; i <= new_cap; ++i) {
        seq_t val = 8 * cache[i-1];
        val -= 28 * cache[i-2];
        val += 56 * cache[i-3];
        val -= 70 * cache[i-4];
        val += 56 * cache[i-5];
        val -= 28 * cache[i-6];
        val += 8 * cache[i-7];
        val -=  cache[i-8];
        cache[i] = val;
    }
    cached_n = new_cap;

    return cache[(size_t)n];
}
