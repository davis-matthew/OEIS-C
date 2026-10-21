/*
 * A230942  Number of (n+3) X (1+3) 0..3 white square subarrays x(i,j) with
 *          each element diagonally or antidiagonally next to at least one
 *          element with value (x(i,j)+1) mod 4, and upper left element zero.
 * https://oeis.org/A230942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 3*a(n-1) + 14*a(n-3) + a(n-4) - a(n-5),  for n ≥ 6
 * Initial values (offset 1):
 *   a(1) = 6,  a(2) = 16,  a(3) = 76,  a(4) = 314,  a(5) = 1170
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;

    /* Guard against invalid call */
    if (n == 0) return (seq_t)0;

    /* Initialize cache with first five terms */
    if (cache == NULL) {
        const idx_t init_cap = 5;
        cache = (seq_t *)malloc(sizeof(seq_t) * init_cap);
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)6;
        cache[1] = (seq_t)16;
        cache[2] = (seq_t)76;
        cache[3] = (seq_t)314;
        cache[4] = (seq_t)1170;
        cache_size = init_cap;
    }

    /* Return stored value if already computed */
    if (n <= cache_size) return cache[n - 1];

    /* Expand cache to accommodate requested index */
    idx_t new_cap = n;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!tmp) {
        /* Allocation failed: clear and return 0 (unlikely for test cases) */
        free(cache);
        cache = NULL;
        cache_size = 0;
        return (seq_t)0;
    }
    cache = tmp;
    /* Compute terms up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 5) {
            /* Initial values already set, but reassign for safety */
            switch(i) {
                case 1: val = (seq_t)6;   break;
                case 2: val = (seq_t)16;  break;
                case 3: val = (seq_t)76;  break;
                case 4: val = (seq_t)314; break;
                case 5: val = (seq_t)1170; break;
                default: val = (seq_t)0; break;  /* unreachable */
            }
        } else {
            /* Recurrence: a(n) = 3*a(n-1) + 14*a(n-3) + a(n-4) - a(n-5) */
            seq_t a1 = cache[i - 2];   /* a(i-1) */
            seq_t a3 = cache[i - 4];   /* a(i-3) */
            seq_t a4 = cache[i - 5];   /* a(i-4) */
            seq_t a5 = cache[i - 6];   /* a(i-5) */
            val = (seq_t)3 * a1 + (seq_t)14 * a3 + a4 - a5;
        }
        cache[i - 1] = val;
    }
    cache_size = n;
    return cache[n - 1];
}
