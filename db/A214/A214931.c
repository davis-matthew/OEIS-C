/*
 * A214931  Number of self-avoiding walks of any length from NW to SW corners
 *          of a grid or lattice with 4 rows and n columns.
 * https://oeis.org/A214931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = 7*a(n-1) - 12*a(n-2) + 7*a(n-3) - 3*a(n-4) - 2*a(n-5)
 *            with initial values a(1)=1, a(2)=8, a(3)=38, a(4)=178, a(5)=844
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[5] = {
    (seq_t)1,
    (seq_t)8,
    (seq_t)38,
    (seq_t)178,
    (seq_t)844
};

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_used = 0;   /* largest index stored in cache (>=5 after first init) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                    /* offset not important */
    if (n <= 5) return init_vals[n-1];

    /* Ensure cache exists and is large enough */
    if (cache == NULL) {
        cache_cap = (n + 1 > 6) ? (n + 1) : 6;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();                 /* out of memory */
        cache[1] = init_vals[0];
        cache[2] = init_vals[1];
        cache[3] = init_vals[2];
        cache[4] = init_vals[3];
        cache[5] = init_vals[4];
        cache_used = 5;
    } else if (cache_cap <= n) {
        cache_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_used + 1; i <= n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;
        pos += (seq_t)7 * cache[i-1];
        pos += (seq_t)7 * cache[i-3];
        neg += (seq_t)12 * cache[i-2];
        neg += (seq_t)3 * cache[i-4];
        neg += (seq_t)2 * cache[i-5];
        cache[i] = pos - neg;    /* pos >= neg holds for all n >= 6 */
    }
    cache_used = n;
    return cache[n];
}
