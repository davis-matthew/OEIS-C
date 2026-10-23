/*
 * A037119  Trajectory of 3 under map n->43n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A037119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=3; for n>0, a(n) = (a(n-1) is odd) ? 43*a(n-1)+1 : a(n-1)/2
 *            (division by 2 is a right shift)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Memoised cache for the trajectory of 3 under the rule */
static seq_t *cache = NULL;        /* array of computed terms */
static size_t cache_capacity = 0;  /* allocated size of cache */
static idx_t  cache_len = 0;       /* number of terms currently stored */

/* Helper to grow the cache to accommodate at least `need` entries */
static void grow_cache(idx_t need)
{
    if (cache_len > need)
        return;               /* already enough */

    if (cache_capacity == 0) {
        size_t new_cap = (size_t)need + 1;
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        cache_capacity = new_cap;
        cache[0] = (seq_t)3;  /* a(0) = 3 */
        cache_len = 1;
    } else if ((size_t)(need + 1) > cache_capacity) {
        size_t new_cap = cache_capacity * 2;
        if (new_cap == 0) new_cap = 1;
        if ((size_t)(need + 1) > new_cap)
            new_cap = need + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Compute missing terms up to `need` */
    for (idx_t i = cache_len; i <= need; ++i) {
        seq_t prev = cache[i - 1];
        seq_t next;
        if (prev & (seq_t)1) {               /* odd */
            next = (prev * (seq_t)43) + (seq_t)1;
        } else {                            /* even */
            next = prev >> 1;                /* divide by 2 */
        }
        cache[i] = next;
    }
    cache_len = need + 1;
}

seq_t a(idx_t n)
{
    if (n < cache_len)
        return cache[n];

    grow_cache(n);
    return cache[n];
}
