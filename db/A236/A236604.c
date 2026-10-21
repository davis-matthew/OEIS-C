/*
 * A236604  Number of (n+1) X (1+1) 0..2 arrays colored with the maximum plus
 *          the lower median minus the minimum of every 2 X 2 subblock.
 * https://oeis.org/A236604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A236604 - Number of (n+1)×2 arrays over {0,1,2} in which every
 *            2×2 subblock satisfies max + lower_median - min = the entry in
 *            the lower‑right corner. The sequence follows the empirical
 *            linear recurrence a(n) = 28 a(n-3) + 336 a(n-4) + 744 a(n-5) +
 *            3360 a(n-6) + 6176 a(n-7) + 8308 a(n-8) + 14576 a...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t max_computed = 0;

    /* initialise cache if first call */
    if (cache == NULL) {
        cache_cap = 64;                     /* enough for the 17 seed values   */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < cache_cap; ++i) cache[i] = 0;
    }

    /* grow cache to hold index n */
    if (n >= cache_cap) {
        size_t new_cap = n + 1;
        if (new_cap < cache_cap * 2) new_cap = cache_cap * 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
        cache_cap = new_cap;
    }

    /* seed base values (only once) */
    if (max_computed == 0) {
        static const seq_t base[18] = {
            0ULL, /* unused 0 index */
            81ULL, 426ULL, 2269ULL, 12004ULL, 63945ULL,
            339684ULL, 1806104ULL, 9590924ULL, 50981860ULL,
            270885808ULL, 1439698020ULL, 7649649584ULL,
            40652677572ULL, 216021080080ULL, 1147974683632ULL,
            6100208729024ULL, 32416974402064ULL
        };
        for (idx_t i = 1; i <= 17; ++i) cache[(size_t)i] = base[(size_t)i];
        max_computed = 17;
    }

    if (n <= max_computed) return cache[(size_t)n];

    /* compute values up to n using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 17) {          /* safety, though should not occur */
            /* base values already set */
            continue;
        }
        seq_t pos = 0, neg = 0;
        pos += cache[(size_t)(i-3)] * 28ULL;
        pos += cache[(size_t)(i-4)] * 336ULL;
        pos += cache[(size_t)(i-5)] * 744ULL;
        pos += cache[(size_t)(i-6)] * 3360ULL;
        pos += cache[(size_t)(i-7)] * 6176ULL;
        pos += cache[(size_t)(i-8)] * 8308ULL;
        pos += cache[(size_t)(i-9)] * 14576ULL;
        pos += cache[(size_t)(i-14)] * 1536ULL;
        pos += cache[(size_t)(i-15)] * 8192ULL;
        pos += cache[(size_t)(i-16)] * 147456ULL;
        pos += cache[(size_t)(i-17)] * 229376ULL;

        neg += cache[(size_t)(i-10)] * 20528ULL;
        neg += cache[(size_t)(i-11)] * 34304ULL;
        neg += cache[(size_t)(i-12)] * 86784ULL;
        neg += cache[(size_t)(i-13)] * 144128ULL;

        cache[(size_t)i] = pos - neg;   /* pos >= neg for all n */
    }
    max_computed = n;

    return cache[(size_t)n];
}
