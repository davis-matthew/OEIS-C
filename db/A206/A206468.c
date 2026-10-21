/*
 * A206468  Number of (n+1)X5 0..2 arrays with every 2X2 subblock having zero
 *          permanent.
 * https://oeis.org/A206468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 11, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A206468:
   a(n) = 39*a(n-1) - 11348*a(n-3) + 76272*a(n-4) + 429792*a(n-5)
          - 3994496*a(n-6) + 37171200*a(n-8) - 43614208*a(n-9)
   (valid for n ≥ 10, with a(1)…a(9) as below) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation, indexed by 1‑based n */
    static seq_t *cache = NULL;     /* array of seq_t */
    static idx_t cache_len = 0;     /* current size of cache (≥1 if cache is not NULL) */

    /* n below the offset: value irrelevant – return 0 */
    if (n == (idx_t)0) return 0;

    /* initialise the cache with the first nine terms once */
    if (cache == NULL) {
        cache_len = 10;                          /* indices 0 … 9, ignore 0 */
        cache = (seq_t*)calloc(cache_len, sizeof(seq_t));
        if (!cache) return 0;                    /* allocation failure */
        cache[1] = (seq_t)1849;
        cache[2] = (seq_t)106891;
        cache[3] = (seq_t)1896129;
        cache[4] = (seq_t)68352739;
        cache[5] = (seq_t)1693734025;
        cache[6] = (seq_t)52552339515;
        cache[7] = (seq_t)1457044540561;
        cache[8] = (seq_t)43214597865811;
        cache[9] = (seq_t)1240051361495769;
    }

    /* if the requested index is already computed, return it */
    if (n < cache_len)
        return cache[n];

    /* expand the cache to hold up to index n */
    seq_t *new_cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) return 0;                 /* allocation failure */
    cache = new_cache;
    /* initialise the newly allocated slots to zero */
    for (idx_t i = cache_len; i <= n; ++i)
        cache[i] = (seq_t)0;

    /* compute new terms using the recurrence */
    for (idx_t k = cache_len; k <= n; ++k) {
        /* recurrence applicable for k ≥ 10 */
        if (k < 10) continue;

        seq_t pos = 0, neg = 0;
        pos += cache[k - 1] * (seq_t)39;
        pos += cache[k - 4] * (seq_t)76272;
        pos += cache[k - 5] * (seq_t)429792;
        pos += cache[k - 8] * (seq_t)37171200;

        neg += cache[k - 3] * (seq_t)11348;
        neg += cache[k - 6] * (seq_t)3994496;
        neg += cache[k - 9] * (seq_t)43614208;

        cache[k] = pos - neg;                  /* unsigned arithmetic */
    }

    cache_len = n + 1;
    return cache[n];
}
