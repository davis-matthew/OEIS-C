/*
 * A093147  Third binomial transform of Pell(3n+1).
 * https://oeis.org/A093147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=15, a(n)=20*a(n-1)-50*a(n-2) for n>=2 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for values that have already been computed. */
    static seq_t *cache   = NULL;   /* array of computed terms          */
    static size_t cache_cap = 0;    /* allocated capacity               */
    static size_t cache_len = 0;    /* number of terms actually defined */

    /* Initialise cache with the first two terms on the very first call. */
    if (cache_len == 0) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) {                /* out of memory – fail gracefully   */
            return (seq_t)0;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)15;
        cache_len = 2;
    }

    /* If the requested term is already known, return it immediately. */
    if ((size_t)n < cache_len) {
        return cache[n];
    }

    /* Grow the cache if necessary.  Double the capacity until it can hold n+1 terms. */
    if ((size_t)n + 1 > cache_cap) {
        size_t newcap = cache_cap ? cache_cap : 1;
        while (newcap <= n) {
            newcap <<= 1;                    /* power‑of‑two growth            */
        }
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) {                            /* allocation failure              */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = newcap;
    }

    /* Compute the remaining terms using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t t = cache[i - 1];
        t *= (seq_t)20;
        t -= cache[i - 2] * (seq_t)50;
        cache[i] = t;
    }
    cache_len = (size_t)n + 1;  /* all terms up to n are now defined */

    return cache[n];
}
