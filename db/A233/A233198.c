/*
 * A233198  Number of n X 4 0..7 arrays with no element x(i,j) adjacent to
 *          itself or value 7-x(i,j) horizontally, antidiagonally or
 *          vertically, top left element zero, and 1 appearing before 2 3 4 5
 *          and 6, 2 appearing before 3 4 and 5, and 3 appearing before 4 in
 *          row major order (unlabelled 8-colorings with n...
 * https://oeis.org/A233198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = recurrence of order 5 a(n) = 208*a(n-1) - 13312*a(n-2) +
 *            442368*a(n-3) - 8650752*a(n-4) + 67108864*a(n-5) for n > 6
 *            Initial values (n = 1…6): 11, 528, 62720, 7802880, 981532672,
 *            123833679872
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Cached sequence values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Largest n already computed */
static size_t max_computed = 0;

/* Initial terms, indexed from 1 to 6 */
static const seq_t init_terms[7] = {
    0ULL,                       /* unused, cache[0] is unused */
    11ULL,                      /* a(1) */
    528ULL,                     /* a(2) */
    62720ULL,                   /* a(3) */
    7802880ULL,                 /* a(4) */
    981532672ULL,               /* a(5) */
    123833679872ULL             /* a(6) */
};

/* Compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    /* Convert to size_t for indexing; n is guaranteed >=1 */
    size_t idx = (size_t)n;

    /* Resize cache if necessary */
    if (idx + 1 > cache_cap) {
        size_t new_cap = idx + 1;
        seq_t *old = cache;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) {      /* allocation failure – abort */
            free(old);
            return 0;
        }
        cache_cap = new_cap;
    }

    /* Compute terms from (max_computed + 1) up to idx */
    for (size_t i = max_computed + 1; i <= idx; ++i) {
        if (i <= 6) {
            cache[i] = init_terms[i];
        } else {
            cache[i] =
                (seq_t)208     * cache[i - 1] -
                (seq_t)13312  * cache[i - 2] +
                (seq_t)442368  * cache[i - 3] -
                (seq_t)8650752 * cache[i - 4] +
                (seq_t)67108864* cache[i - 5];
        }
    }

    max_computed = idx;
    return cache[idx];
}
