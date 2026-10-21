/*
 * A228545  y-values in the solution to the Pell equation x^2 - 61*y^2 = -1.
 * https://oeis.org/A228545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3532638098*a(n-1) - a(n-2), a(1)=3805,
 *            a(2)=13441687959085
 * Range:     u8 none, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* The sequence is defined by a linear recurrence.
       We memoize the values in a static array so that
       calls with arbitrary n order are efficient. */
    static seq_t *cache = NULL;   /* array of terms, cache[1] = a(1) */
    static size_t cap = 0;        /* allocated size (including unused index 0) */
    static size_t last = 0;       /* last computed index (>=0) */

    /* Ensure enough capacity for index n   (cache[0] unused) */
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? cap * 2 : 4;
        if (new_cap <= (size_t)n) new_cap = n + 2;  /* need at least n+2 slots */

        seq_t *new_cache = (seq_t *)calloc(new_cap, sizeof(seq_t));
        if (!new_cache) return 0;   /* fallback on allocation failure */
        if (cache) {
            memcpy(new_cache, cache, cap * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cap = new_cap;

        /* Initialize the two seed values if they have not been set. */
        if (last < 1) {
            cache[1] = (seq_t)3805;
            last = 1;
        }
        if (last < 2) {
            cache[2] = (seq_t)13441687959085ULL;
            last = 2;
        }
    }

    /* Compute missing terms up to n. */
    while (last < (size_t)n) {
        size_t i = last + 1;
        cache[i] = (seq_t)3532638098ULL * cache[i - 1] - cache[i - 2];
        last = i;
    }

    return cache[n];
}
