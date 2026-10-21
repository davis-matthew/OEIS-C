/*
 * A062882  a(n) = (1 - 2*cos(Pi/9))^n + (1 + 2*cos(Pi*2/9))^n + (1 +
 *          2*cos(Pi*4/9))^n.
 * https://oeis.org/A062882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(a(n-1)-a(n-3)), with a(1)=3, a(2)=9, a(3)=18
 *            (recurrence derived from the closed‑form expression)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 47, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoization */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of terms stored */

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n<1 – return 0 */
        return (seq_t)0;

    /* use cached value if available */
    if (n <= cache_len)
        return cache[n - 1];

    /* resize cache to hold up to n terms */
    if (cache_len < n) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp)        /* allocation failure – return 0 */
            return (seq_t)0;
        cache = tmp;
    }

    /* initialise the first three terms if not yet present */
    if (cache_len == 0) {
        cache[0] = (seq_t)3ULL;
        cache_len = 1;
    }
    if (cache_len == 1 && n >= 2) {
        cache[1] = (seq_t)9ULL;
        cache_len = 2;
    }
    if (cache_len == 2 && n >= 3) {
        cache[2] = (seq_t)18ULL;
        cache_len = 3;
    }

    /* compute missing terms onward */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* i-th term uses 1‑based indices: a(i-1) = cache[i-2],
           a(i-3) = cache[i-4] */
        seq_t val = (seq_t)3 * (cache[i - 2] - cache[i - 4]);
        cache[i - 1] = val;
    }

    cache_len = n;
    return cache[n - 1];
}
