/*
 * A207591  Number of 4 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 1 1 1 vertically.
 * https://oeis.org/A207591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-2) + 2*a(n-3) - 10*a(n-4) + 2*a(n-6) for n > 8
 *            Initial values: a(1)=9, a(2)=81, a(3)=144, a(4)=432, a(5)=1098,
 *            a(6)=3150, a(7)=8244, a(8)=23202 The recurrence holds for all n
 *            > 8. The function memoizes results so that subsequent calls with
 *            arbitrary indices run in O(max(n)) time
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 20, u64 n <= 42, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;    /* allocated capacity */
static idx_t computed_max = 0; /* last index that has been computed */

/* helper: initial values (index 0 unused) */
static const seq_t init_vals[9] = {0,9,81,144,432,1098,3150,8244,23202};

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n=0, return 0 as placeholder */

    /* grow cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;

        /* initialise known small values */
        idx_t upto = cache_cap - 1;
        if (upto > 8) upto = 8;
        for (idx_t i = 1; i <= upto; ++i) cache[i] = init_vals[i];
        computed_max = upto;
    }

    /* compute any missing terms up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        if (i <= 8) {
            cache[i] = init_vals[i];
        } else {
            seq_t val = (seq_t)8 * cache[i-2]
                       + (seq_t)2 * cache[i-3]
                       - (seq_t)10 * cache[i-4]
                       + (seq_t)2 * cache[i-6];
            cache[i] = val;
        }
    }
    computed_max = n;

    return cache[n];
}
