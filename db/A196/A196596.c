/*
 * A196596  Number of n X 3 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 1,2,4,0,3
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) + 3*a(n-3) + a(n-4) - a(n-5) with
 *            initial values a(1)=1, a(2)=3, a(3)=16, a(4)=37, a(5)=116
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, free */

/* Static cache for memoisation */
static seq_t *cache   = NULL;   /* holds a(0) .. a(max_n), a(0) is unused */
static idx_t  cache_cap = 0;    /* current capacity of the cache (max_n+1) */
static idx_t  computed  = 0;    /* highest index for which value is computed */

/* Initial values for n = 0..5 (a(0) is unused, set to 0 for convenience) */
static const seq_t init_vals[6] = {0, (seq_t)1, (seq_t)3, (seq_t)16,
                                   (seq_t)37, (seq_t)116};

seq_t a(idx_t n) {
    /* a(0) is outside the official offset – return 0 for safety */
    if (n == 0) return (seq_t)0;

    /* Expand cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;        /* allocator failure: return 0 */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialise missing initial values up to 5 */
    if (computed < 5) {
        idx_t up_to = (n < 5) ? n : 5;
        for (idx_t i = computed + 1; i <= up_to; ++i)
            cache[i] = init_vals[i];
        computed = up_to;
    }

    /* Compute further values up to n */
    if (computed < n) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            if (i <= 5) {
                cache[i] = init_vals[i];   /* safety for n <= 5 */
            } else {
                /* Use temporary variables to avoid overflow of intermediate sums */
                seq_t t1 = (seq_t)2 * cache[i-1];
                seq_t t2 = (seq_t)2 * cache[i-2];
                seq_t t3 = (seq_t)3 * cache[i-3];
                seq_t t4 =        cache[i-4];
                seq_t t5 =        cache[i-5];
                cache[i] = t1 + t2 + t3 + t4 - t5;
            }
        }
        computed = n;
    }

    return cache[n];
}
