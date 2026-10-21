/*
 * A221949  Expansion of (-x+2*x^2-x^3-x^4-2*x^5)/(-1+3*x-2*x^2-x^4+x^5).
 * https://oeis.org/A221949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) - 2*a(n-2) - a(n-4) + a(n-5)  for n ≥ 6
   Initial values: a(0)=0, a(1)=1, a(2)=1, a(3)=2, a(4)=5, a(5)=12 */
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t  cache_len = 0;

static const seq_t init[6] = {
    (seq_t)0,   /* a(0) */
    (seq_t)1,   /* a(1) */
    (seq_t)1,   /* a(2) */
    (seq_t)2,   /* a(3) */
    (seq_t)5,   /* a(4) */
    (seq_t)12   /* a(5) */
};

seq_t a(idx_t n)
{
    /* allocate (or reallocate) space up to n */
    size_t target = (size_t)n + 1;
    if (cache_len < target)
    {
        size_t old_len = cache_len;
        seq_t *tmp = (seq_t *)realloc(cache, target * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                /* allocation failure: fallback   */
        cache = tmp;
        cache_len = target;

        /* initialize known values if the array grew to contain them   */
        if (old_len < 6)
        {
            size_t init_len = (6 < target) ? 6 : target;
            for (size_t i = old_len; i < init_len; ++i)
                cache[i] = init[i];
        }

        /* compute new terms using the recurrence                 */
        size_t start = (old_len > 6) ? old_len : 6;
        for (size_t i = start; i <= (size_t)n; ++i)
        {
            cache[i] = (seq_t)3 * cache[i-1]
                       - (seq_t)2 * cache[i-2]
                       - cache[i-4]
                       + cache[i-5];
        }
    }
    return cache[n];
}
