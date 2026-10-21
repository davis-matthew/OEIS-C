/*
 * A070992  Partial sums of A002487.
 * https://oeis.org/A070992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Formula implemented:
   a(0) = 1, a(1) = 3, a(2) = 4
   For n ≥ 1:
     a(2n)   = a(n-2) + 2*a(n-1) + 2
     a(2n+1) = 2*a(n-1) + a(n)   + 2
*/
#include <stdlib.h>

static seq_t *cache = NULL;      /* memoised terms */
static idx_t max_cached = 0;     /* highest index cached so far */

/* ensure cache is large enough and initialise base values if necessary */
static void ensure_cache(idx_t n)
{
    if (n < 3)
        return;                 /* already covered by base values */

    if (max_cached == 0) {
        /* first time: allocate space for indices 0..2 */
        cache = (seq_t *)malloc(sizeof(seq_t) * 3);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)4;
        max_cached = 2;
    }

    if (n <= max_cached)
        return;                 /* already have enough */

    /* grow cache to size n+1 */
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
    if (!new_cache) return;     /* allocation failure, but caller will handle */
    cache = new_cache;

    /* compute new terms */
    const seq_t two = (seq_t)2;
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        if (i & 1) {            /* odd: i = 2k+1, k = (i-1)/2 */
            idx_t k = (i - 1) >> 1;
            cache[i] = two * cache[k - 1] + cache[k] + two;
        } else {                /* even: i = 2m, m = i/2 */
            idx_t m = i >> 1;
            cache[i] = cache[m - 2] + two * cache[m - 1] + two;
        }
    }
    max_cached = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)4;

    ensure_cache(n);
    return cache[n];
}
