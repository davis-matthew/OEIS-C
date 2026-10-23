/*
 * A173593  Numbers having in binary representation exactly two ones in three
 *          consecutive digits.
 * https://oeis.org/A173593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2*a(n-2) + a(n-3) – 2*a(n-5), a(1)=3, a(2)=5, a(3)=6,
 *            a(4)=11, a(5)=13
 * Range:     u8 n <= 13, u16 n <= 29, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cache for memoisation (indices start at 1, position 0 unused). */
static seq_t *cache = NULL;
static size_t cache_len = 0;          /* number of allocated slots (>= 1) */
static idx_t last_computed = 0;       /* highest n that has a value in cache */

/* Ensure that cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n) {
    size_t required = (size_t)n + 1;      /* +1 because indices start at 1 */
    if (required > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – terminate gracefully */
            exit(EXIT_FAILURE);
        }
        /* initialise the new part to zero (or any value – never used yet) */
        for (size_t i = cache_len; i < required; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_len = required;
    }
}

/* Initialise base values a(1) … a(5). */
static void init_cache(void) {
    ensure_cache(5);
    cache[0] = 0;              /* unused, keeps indexing simple */
    cache[1] = (seq_t)3;
    cache[2] = (seq_t)5;
    cache[3] = (seq_t)6;
    cache[4] = (seq_t)11;
    cache[5] = (seq_t)13;
    last_computed = 5;
}

seq_t a(idx_t n) {
    if (cache_len == 0)           /* first call: initialise cache */
        init_cache();

    if (n <= last_computed)       /* already computed – return directly */
        return cache[(size_t)n];

    /* Extend cache up to n */
    ensure_cache(n);

    /* Iteratively compute missing values using the recurrence */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* Work with idx_t cast to size_t for array indexing */
        size_t si = (size_t)i;

        /* a(i) = 2*a(i-2) + a(i-3) - 2*a(i-5) */
        seq_t value = ((seq_t)2) * cache[si - 2];
        value += cache[si - 3];
        value -= ((seq_t)2) * cache[si - 5];

        cache[si] = value;
    }

    last_computed = n;
    return cache[(size_t)n];
}
