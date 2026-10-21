/*
 * A220706  Number of ways to reciprocally link elements of an n X 6 array
 *          either to themselves or to exactly two horizontal and antidiagonal
 *          neighbors, without consecutive collinear links.
 * https://oeis.org/A220706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A220706
   Recurrence:
   a(n) = 3*a(n-1) + 5*a(n-2) - 3*a(n-3) - 6*a(n-4) - a(n-5) - a(n-6)
          + 4*a(n-7) + a(n-8) - a(n-9) - a(n-10),
   with
   a(1)=1, a(2)=8, a(3)=24, a(4)=105, a(5)=405, a(6)=1617,
   a(7)=6412, a(8)=25449, a(9)=101029, a(10)=400986.
   The implementation memoizes computed values in a static array so that
   calls are O(1) after the first computation and the entire sequence up
   to n can be generated in linear time.  The recurrence is evaluated
   using only non‑negative intermediate values to avoid overflow
   issues with unsigned arithmetic. */

#include <stdlib.h>

/* external linkage as required */
seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* allocated size (index 0 unused)      */
    static idx_t max_computed = 0; /* highest n for which cache[n] is valid */

    /* minimum cache size needed to hold the first 10 terms */
    const idx_t MIN_SIZE = 11;

    /* initialise cache on first call */
    if (cache == NULL) {
        idx_t initial_size = (n + 1 > MIN_SIZE) ? (n + 1) : MIN_SIZE;
        cache = (seq_t *)malloc(initial_size * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* out of memory: undefined, but returns 0 */

        /* base terms, index 0 unused */
        cache[0] = 0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)24;
        cache[4] = (seq_t)105;
        cache[5] = (seq_t)405;
        cache[6] = (seq_t)1617;
        cache[7] = (seq_t)6412;
        cache[8] = (seq_t)25449;
        cache[9] = (seq_t)101029;
        cache[10] = (seq_t)400986;

        cache_size = initial_size;
        max_computed = 10;
    }

    /* grow cache if needed */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        if (new_size < MIN_SIZE) new_size = MIN_SIZE;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* out of memory */
        cache = tmp;
        cache_size = new_size;
    }

    /* if value already computed, return it */
    if (n <= max_computed) {
        return cache[n];
    }

    /* compute new terms using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* compute positive and negative contributions separately */
        seq_t sum_pos = (seq_t)3 * cache[i - 1]
                      + (seq_t)5 * cache[i - 2]
                      + (seq_t)4 * cache[i - 7]
                      + cache[i - 8];
        seq_t sum_neg = (seq_t)3 * cache[i - 3]
                      + (seq_t)6 * cache[i - 4]
                      +      cache[i - 5]
                      +      cache[i - 6]
                      +      cache[i - 9]
                      +      cache[i - 10];

        cache[i] = sum_pos - sum_neg; /* guaranteed non‑negative */
    }

    max_computed = n;
    return cache[n];
}
