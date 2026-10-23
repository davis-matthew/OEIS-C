/*
 * A169652  a(1) = 5, a(n) = square of sum of previous terms.
 * https://oeis.org/A169652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 5 a(n+1) = (sum_{i=1}^n a(i))^2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;          /* array of a(i) */
static size_t capacity = 0;          /* allocated capacity in elements */
static idx_t computed = 0;           /* number of terms already computed */
static seq_t sum = 0;                /* sum of all computed terms */

seq_t a(idx_t n)
{
    /* n < 1 is outside the OEIS offset – return 0 for convenience */
    if (n == 0)
        return 0;

    /* Ensure the cache has enough space for n terms */
    if ((size_t)n > capacity) {
        size_t new_cap = capacity ? capacity * 2 : 8;
        while (new_cap < (size_t)n)
            new_cap *= 2;

        /* Cast is required for C++ compatibility */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – terminate */
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute missing terms up to index n */
    for (idx_t i = computed; i < n; ++i) {
        seq_t val;
        if (i == 0) {
            /* a(1) = 5 */
            val = 5;
        } else {
            /* a(i+1) = (sum of previous terms)^2 */
            val = sum * sum;
        }
        cache[i] = val;
        sum += val;   /* update cumulative sum */
    }

    computed = n;          /* all terms up to n are now cached */
    return cache[n - 1];
}
