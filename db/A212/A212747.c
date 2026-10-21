/*
 * A212747  Number of (w,x,y,z) with all terms in {0,...,n} and
 *          2w=floor((x+y+z)/2).
 * https://oeis.org/A212747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n)=4*a(n-1)-7*a(n-2)+8*a(n-3)-7*a(n-4)+4*a(n-5)-a(n-6) */
/* Initial values: a(0)=1, a(1)=4, a(2)=13, a(3)=32, a(4)=63, a(5)=108, a(6)=171 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t computed = 0;

seq_t a(idx_t n)
{
    if (cache == NULL) {
        cache_cap = 7;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                      /* Out of memory – return 0 */
        cache[0] = 1; cache[1] = 4; cache[2] = 13;
        cache[3] = 32; cache[4] = 63; cache[5] = 108; cache[6] = 171;
        computed = 6;
    }

    if (n < cache_cap)
        return cache[n];

    /* Grow the cache if needed */
    if ((idx_t)(n + 1) > cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;                  /* Out of memory – return 0 */
        cache = new_cache;
        cache_cap = new_cap;
    }

    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t pos = ((seq_t)4 * cache[i-1]) +
                    ((seq_t)8 * cache[i-3]) +
                    ((seq_t)4 * cache[i-5]);

        seq_t neg = ((seq_t)7 * cache[i-2]) +
                    ((seq_t)7 * cache[i-4]) +
                    cache[i-6];

        cache[i] = pos - neg; /* pos >= neg for all n >= 6 */
    }
    computed = n;
    return cache[n];
}
