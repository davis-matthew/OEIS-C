/*
 * A170167  Number of reduced words of length n in Coxeter group on 14
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^39 = I.
 * https://oeis.org/A170167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  Formula: a(n) is defined by the rational generating function
 *  (t^39 + 2*t^38 + 2*t^37 + … + 2*t + 1) /
 *  (78*t^39 - 12*t^38 - 12*t^37 - … - 12*t + 1).
 *
 *  Consequently a(0)=1 and for 1≤n≤38
 *      a(n) = 2 + 12*Σ_{k=0}^{n-1} a(k)
 *  for n≥39 the linear recurrence
 *      a(n) = 12*Σ_{k=n-38}^{n-1} a(k) – 78*a(n-39)
 *  holds.
 *
 *  This implementation caches computed terms so that each n≥0 is
 *  evaluated only once, in O(n) time overall.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of computed terms */
    static size_t cache_size = 0; /* allocated size */
    static idx_t  max_cached = (idx_t)-1; /* highest index stored */

    /* Initial allocation and computation of first 0…38 */
    if (!cache) {
        cache_size = 40; /* at least up to index 39 will be needed */
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failure */

        /* compute a(0)…a(38) using the defining formula */
        cache[0] = (seq_t)1;
        seq_t cumulative = cache[0];      /* Σ_{k=0}^{i-1} a(k) */
        for (idx_t i = 1; i <= 38; ++i) {
            cache[i] = (seq_t)(2 + (seq_t)12 * cumulative);
            cumulative += cache[i];
        }
        max_cached = 38;
    }

    /* Grow the cache if necessary */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* If we already have the answer, return it */
    if (max_cached >= n)
        return cache[n];

    /* For n ≥ 39 compute the sequence iteratively */
    /* Prepare the sliding sum of the last 38 terms: Σ_{k=1}^{38} a(k)  */
    seq_t sum38 = (seq_t)0;
    for (idx_t j = 1; j <= 38; ++j)
        sum38 += cache[j];

    /* compute a(39) first */
    seq_t a39 = (seq_t)( (seq_t)12 * sum38 - (seq_t)78 * cache[0] );
    cache[39] = a39;
    max_cached = 39;

    /* update sum38 for the next index (n=40) */
    sum38 += a39 - cache[1];

    /* Continue up to n */
    for (idx_t i = 40; i <= n; ++i) {
        seq_t ai = (seq_t)( (seq_t)12 * sum38 - (seq_t)78 * cache[i-39] );
        cache[i] = ai;
        /* shift the window: remove a(i-38), add a(i) */
        sum38 += ai - cache[i-38];
        ++max_cached;
    }

    return cache[n];
}
