/*
 * A081035  8th binomial transform of the periodic sequence (1,9,1,1,9,1...).
 * https://oeis.org/A081035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*9^n - 4*7^n (equivalent) Recurrence: a(0)=1, a(1)=17,
 *            a(n)=16*a(n-1)-63*a(n-2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;    /* memoised values */
static idx_t cache_len = 0;    /* number of elements currently stored */

seq_t a(idx_t n)
{
    /* base values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)17;

    /* initialise cache if first time */
    if (cache_len == 0) {
        /* allocate space for two elements */
        cache = (seq_t*)malloc(2 * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* out‑of‑memory: return 0 as safe fallback */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)17;
        cache_len = 2;
    }

    /* if required value already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* extend cache up to required n */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return (seq_t)0;    /* allocation failure */
    cache = tmp;

    /* compute missing values using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* a(i) = 16*a(i-1) - 63*a(i-2) */
        cache[i] = (seq_t)(16 * cache[i - 1] - 63 * cache[i - 2]);
    }
    cache_len = new_len;
    return cache[n];
}
