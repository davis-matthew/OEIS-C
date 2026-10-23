/*
 * A025951  Expansion of 1/((1-2*x)*(1-3*x)*(1-8*x)*(1-12*x)).
 * https://oeis.org/A025951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*a(n-1) - 202*a(n-2) + 600*a(n-3) - 576*a(n-4) Initial
 *            values: a(0)=1, a(1)=25, a(2)=423, a(3)=6125
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoized values */
    static seq_t *cache = NULL;      /* array of computed terms */
    static idx_t cache_cap = 0;      /* allocated capacity */
    static idx_t cache_len = 0;      /* number of terms already computed */

    /* Determine required capacity: at least 4 for the initial values */
    idx_t need_cap = (n + 1 > 4 ? n + 1 : 4);

    /* Allocate or grow the cache as needed */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t) * need_cap);
        if (!cache)
            return (seq_t)0;          /* allocation failed */
        cache_cap = need_cap;
        /* initialise the first four terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)25;
        cache[2] = (seq_t)423;
        cache[3] = (seq_t)6125;
        cache_len = 4;
    } else if (cache_cap < need_cap) {
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * need_cap);
        if (!tmp)
            return (seq_t)0;          /* realloc failed */
        cache = tmp;
        cache_cap = need_cap;
        /* note: cache_len remains unchanged; values beyond cache_len were not yet computed */
    }

    /* Compute missing terms up to n (if any) */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        cache[i] = (seq_t)25 * a1 - (seq_t)202 * a2 + (seq_t)600 * a3 - (seq_t)576 * a4;
    }

    /* Update the number of computed terms */
    if (n + 1 > cache_len)
        cache_len = n + 1;

    return cache[n];
}
