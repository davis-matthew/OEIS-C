/*
 * A065929  (x,y) = (a(n),a(n+1)) are the solutions of (t(x)+t(y))/(1+xy) =
 *          t(3) = 6, where t(n) denotes the n-th triangular number t(n) =
 *          n(n+1)/2.
 * https://oeis.org/A065929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A065929
   Recurrence: a(0)=3, a(1)=35, a(2)=416
   a(n) = 13*a(n-1) - 13*a(n-2) + a(n-3)  for n >= 3
*/

#include <stdlib.h>          /* malloc, realloc */

static seq_t *cache = NULL;   /* memoised terms */
static idx_t cache_size = 0;  /* number of cached entries (>=0) */

seq_t a(idx_t n)
{
    /* Handle the first three terms without lookup */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)35;
    if (n == 2) return (seq_t)416;

    /* Initialise cache if this is the first call or cache too small */
    if (cache == NULL) {
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* unlikely, just in case */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)35;
        cache[2] = (seq_t)416;
        cache_size = 3;
    }

    /* If requested term already cached, return it */
    if (n < cache_size) {
        return cache[(size_t)n];
    }

    /* Grow cache to accommodate up to n */
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (new_cache == NULL) {
        /* reallocation failed; fall back to recomputation without caching */
        seq_t a0 = (seq_t)3, a1 = (seq_t)35, a2 = (seq_t)416, next;
        for (idx_t i = 3; i <= n; ++i) {
            next = 13 * a2;
            next -= 13 * a1;
            next += a0;
            a0 = a1;
            a1 = a2;
            a2 = next;
        }
        return a2;
    }
    cache = new_cache;

    /* Compute missing terms using the recurrence */
    seq_t a0 = cache[cache_size - 3];
    seq_t a1 = cache[cache_size - 2];
    seq_t a2 = cache[cache_size - 1];
    seq_t next;

    for (idx_t i = cache_size; i <= n; ++i) {
        next = 13 * a2;   /* 13*a(i-1) */
        next -= 13 * a1;  /* -13*a(i-2) */
        next += a0;       /* +a(i-3) */

        cache[(size_t)i] = next;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    cache_size = n + 1;

    return cache[(size_t)n];
}
