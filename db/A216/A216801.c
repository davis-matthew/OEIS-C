/*
 * A216801  a(n) = 13*a(n-1) - 65*a(n-2) + 156*a(n-3) - 182*a(n-4) + 91*a(n-5)
 *          - 13*a(n-6).
 * https://oeis.org/A216801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 13*a(n-1) - 65*a(n-2) + 156*a(n-3) - 182*a(n-4) +
 *            91*a(n-5) - 13*a(n-6) Initial values: a(1)=2, a(2)=-22,
 *            a(3)=-117, a(4)=-468, a(5)=-1755, a(6)=-6513
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 15, i64 n <= 31, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* static cache for memoisation */
static seq_t *cache = NULL;        /* 1‑based indexing, cache[0] unused   */
static idx_t cache_size = 0;       /* highest index currently stored      */

/* returns a(n) for n >= 1  */
seq_t a(idx_t n)
{
    /* Handle the trivial cases */
    if (n <= 6) {
        static const seq_t base[7] = { 0, 2, -22, -117, -468, -1755, -6513 };
        return base[n];
    }

    /* Initialise the cache if this is the first call */
    if (!cache) {
        /* Allocate space for indices 0 … 6   (unused index 0) */
        cache = (seq_t*)malloc((size_t)7 * sizeof(seq_t));
        if (!cache) abort();
        cache[1] = 2;
        cache[2] = -22;
        cache[3] = -117;
        cache[4] = -468;
        cache[5] = -1755;
        cache[6] = -6513;
        cache_size = 6;
    }

    /* Allocate more space if necessary */
    if (n > cache_size) {
        size_t new_cap = (size_t)(n + 1);   /* we keep index 0 unused */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        /* Compute values that are not yet cached */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            /* All arithmetic is done in seq_t. */
            cache[i] = 13 * cache[i-1]
                     - 65 * cache[i-2]
                     +156 * cache[i-3]
                     -182 * cache[i-4]
                     + 91 * cache[i-5]
                     - 13 * cache[i-6];
        }
        cache_size = n;
    }

    return cache[n];
}
