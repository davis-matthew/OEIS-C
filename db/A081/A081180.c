/*
 * A081180  4th binomial transform of (0,1,0,2,0,4,0,8,0,16,...).
 * https://oeis.org/A081180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*a(n-1) - 14*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoisation table for a(n) */
static idx_t cache_size = 0;  /* actual number of elements allocated */

/* External linkage as requested */
seq_t a(idx_t n)
{
    /* First call – create a table of at least 2 entries */
    if (cache == NULL) {
        idx_t init_sz = (n > 1 ? n + 1 : 2);          /* guarantee a[1] exists when needed */
        cache = (seq_t*)malloc(init_sz * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;           /* allocation failure */

        cache[0] = (seq_t)0;
        if (init_sz > 1) cache[1] = (seq_t)1;
        cache_size = init_sz;

        /* Pre‑compute up to n (only if n>=2) */
        for (idx_t i = 2; i <= n; ++i) {
            cache[i] = cache[i-1] * (seq_t)8 - cache[i-2] * (seq_t)14;
        }
        return cache[n];
    }

    /* Subsequent calls – use existing table if already large enough */
    if (n < cache_size) {
        return cache[n];
    }

    /* Need to extend the table to include index n */
    idx_t new_sz = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_sz * sizeof(seq_t));
    if (tmp == NULL) return (seq_t)0;           /* reallocation failure */
    cache = tmp;

    /* Compute the new entries */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = cache[i-1] * (seq_t)8 - cache[i-2] * (seq_t)14;
    }
    cache_size = new_sz;

    return cache[n];
}
