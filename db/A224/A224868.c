/*
 * A224868  a(1) = greatest k such that H(k) - H(4) < 1/3 + 1/4; a(2) =
 *          greatest k such that H(k) - H(a(1)) < H(a(1)) - H(4); and for n >
 *          2, a(n) = greatest k such that H(k) - H(a(n-1)) > H(a(n-1)) -
 *          H(a(n-2)), where H = harmonic number.
 * https://oeis.org/A224868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - a(n-4) (conjectured, with
 *            initial values: a(1)=7, a(2)=11, a(3)=17, a(4)=26)
 * Range:     u8 n <= 9, u16 n <= 24, u32 n <= 53, u64 n <= 111, u128 n <= 227,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Array of previously computed terms.
   Index 0 is unused to keep 1‑based indexing. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* last computed index */
static idx_t cache_cap = 0;   /* allocated capacity, including unused 0 */

seq_t a(idx_t n)
{
    /* Handle indices below the offset – behaviour is not specified */
    if (n < 1) return 0;

    /* Initial values (offset 1) */
    static const seq_t init[4] = {7, 11, 17, 26};

    /* Initialize cache on first call */
    if (cache == NULL) {
        cache_cap = 5;                 /* indices 0..4 inclusive */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) { /* allocation failure – abort */
            return 0;
        }
        cache[0] = 0;                   /* unused */
        cache[1] = init[0];
        cache[2] = init[1];
        cache[3] = init[2];
        cache[4] = init[3];
        cache_len = 4;
    }

    /* If the requested index is not yet computed, extend and compute */
    if (n > cache_len) {
        idx_t new_cap = n + 1;          /* need indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {             /* allocation failure – abort */
            return 0;
        }
        cache = tmp;
        cache_cap = new_cap;

        for (idx_t i = cache_len + 1; i <= n; ++i) {
            /* recurrence: a_i = 2*a_{i-1} - a_{i-2} + a_{i-3} - a_{i-4} */
            cache[i] = (2 * cache[i-1]) - cache[i-2] + cache[i-3] - cache[i-4];
        }
        cache_len = n;
    }

    return cache[n];
}
