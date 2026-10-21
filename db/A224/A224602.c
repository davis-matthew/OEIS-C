/*
 * A224602  Number of (n+2) X 6 0..2 matrices with each 3 X 3 subblock
 *          idempotent.
 * https://oeis.org/A224602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 5*a(n-3) - 4*a(n-4) + 3*a(n-5) -
 *            a(n-6) Initial values (n=1..8): 416, 449, 571, 666, 906, 1247,
 *            1657, 2280. Recurrence is valid for n > 8
 * Range:     u8 none, u16 n <= 17, u32 n <= 46, u64 n <= 104, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* allocated size – 1 (index 0 unused) */
    static idx_t max_comp  = 0;  /* highest index computed so far */

    /* Initialise first eight terms if not yet done */
    if (max_comp < 8) {
        /* Ensure storage for indices 0..8 */
        idx_t need = 8;
        if (cache == NULL) {
            cache = (seq_t *)malloc((need + 1) * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, (need + 1) * sizeof(seq_t));
        }
        cache_len = need;

        /* Fixed initial values */
        static const seq_t init[9] = {
            0,      /* unused, keeps cache[0] harmless */
            416, 449, 571, 666, 906, 1247, 1657, 2280
        };
        for (idx_t i = 1; i <= 8; ++i)
            cache[i] = init[i];

        max_comp = 8;
    }

    /* Simple lookup for the first eight terms */
    if (n <= 8)
        return cache[n];

    /* Ensure the cache can store up to index n */
    if (cache_len < n) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        cache_len = n;
    }

    /* Compute new terms using the recurrence, avoiding
       signed arithmetic by separating positive and negative parts. */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        seq_t pos = ((seq_t)4) * cache[i - 1];
        pos += ((seq_t)5) * cache[i - 3];
        pos += ((seq_t)3) * cache[i - 5];

        seq_t neg = ((seq_t)6) * cache[i - 2];
        neg += ((seq_t)4) * cache[i - 4];
        neg += cache[i - 6];

        cache[i] = pos - neg;  /* pos >= neg for all n > 8 */
    }
    max_comp = n;

    return cache[n];
}
