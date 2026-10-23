/*
 * A188098  Number of (n+3) X 5 binary arrays with every 4 X 4 subblock
 *          commuting with each horizontal and vertical neighbor 4 X 4
 *          subblock.
 * https://oeis.org/A188098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 21, u32 n <= 66, u64 n <= 156, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A188098: Count of (n+3)×5 binary arrays where every 4×4 subblock
 *  commutes with its horizontal and vertical neighbours.
 *
 *  Recurrence (valid for n ≥ 16):
 *      a(n) = 2*a(n-1) - a(n-2) + a(n-4) - 2*a(n-5)
 *             + a(n-6) + a(n-8) - a(n-9)
 *
 *  Initial terms (n = 1 … 15):
 *      708, 143, 243, 643, 1482, 2374, 2866, 3110,
 *      3322, 3542, 3834, 4523, 6132, 8705, 11842
 *
 *  The function memoises previously computed values and works in
 *  O(n) time per computation of a new term.
 */

#include <stdlib.h>

static const seq_t init_vals[16] = {
    0,                      /* dummy for 0-index */
    708, 143, 243, 643, 1482, 2374, 2866, 3110,
    3322, 3542, 3834, 4523, 6132, 8705, 11842
};

static seq_t *cache = NULL;   /* memoised sequence values */
static idx_t cache_size = 0;  /* allocated size (index 0 … cache_size) */

/* Return the nth term of A188098 (n ≥ 1). */
seq_t a(idx_t n)
{
    /* initialise the cache on first call */
    if (cache == NULL) {
        cache_size = 15;                     /* we need indices up to 15 */
        cache = (seq_t *)malloc((cache_size + 1) * sizeof(seq_t));
        for (idx_t i = 0; i <= cache_size; ++i)
            cache[i] = 0;                     /* clear element 0 */
        for (idx_t i = 1; i <= cache_size; ++i)
            cache[i] = init_vals[i];
    }

    /* grow the cache if the required n is larger */
    if (n > cache_size) {
        idx_t new_size = n;
        cache = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
        cache_size = new_size;
    }

    /* compute any missing values using the recurrence */
    for (idx_t i = 16; i <= n; ++i) {
        /* i-9 ≥ 7, so all referenced indices are defined */
        seq_t val = 2 * cache[i - 1];
        val -= cache[i - 2];
        val += cache[i - 4];
        val -= 2 * cache[i - 5];
        val += cache[i - 6];
        val += cache[i - 8];
        val -= cache[i - 9];
        cache[i] = val;
    }

    return cache[n];
}
