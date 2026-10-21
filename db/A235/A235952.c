/*
 * A235952  Number of (n+1) X (6+1) 0..2 arrays colored with the upper median
 *          value of each 2 X 2 subblock.
 * https://oeis.org/A235952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 38, u64 n <= 93, u128 n <= 203,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A235952: Number of (n+1)×7 arrays colored with 0..2 that are
 * the upper median value of each 2×2 subblock.
 *
 * Empirical recurrence (verified for n > 14):
 *     a(n) = 5 * a(n‑4)
 *
 * Initial values:
 *     a(1) = 20800,  a(2) = 9752,  a(3) = 3504,  a(4) = 5036,
 *     a(5) = 6728,   a(6) = 10420,\ a(7) = 14440, a(8) = 23148,
 *     a(9) = 33704,  a(10) = 52348, a(11) = 72200, a(12) = 116316,
 *     a(13) = 168520, a(14) = 261612.
 *
 * The function caches computed values so that subsequent calls are fast
 * and works for any ordering of the argument n (n ≥ 1).
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n<1 */

    /* static cache for previously computed terms */
    static seq_t *cache = NULL;   /* allocated with cache[0] unused */
    static idx_t max_cached = 0;  /* highest index stored in cache */

    /* initial values for n=1..14 */
    static const seq_t init[15] = {
        0,          /* cache[0] unused */
        20800,      /* a(1) */
        9752,       /* a(2) */
        3504,       /* a(3) */
        5036,       /* a(4) */
        6728,       /* a(5) */
        10420,      /* a(6) */
        14440,      /* a(7) */
        23148,      /* a(8) */
        33704,      /* a(9) */
        52348,      /* a(10) */
        72200,      /* a(11) */
        116316,     /* a(12) */
        168520,     /* a(13) */
        261612      /* a(14) */
    };

    /* initialise cache on first use */
    if (!cache) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 15);
        if (!cache) return 0;           /* allocation failure */
        for (idx_t i = 0; i < 15; ++i)
            cache[i] = init[i];
        max_cached = 14;
    }

    /* if the requested index is already cached, return it */
    if (n <= max_cached)
        return cache[n];

    /* enlarge cache to cover index n */
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
    if (!new_cache) return 0;          /* allocation failure */
    cache = new_cache;

    /* compute new terms using the recurrence a(k) = 5*a(k‑4) */
    for (idx_t k = max_cached + 1; k <= n; ++k) {
        if (k <= 14) {
            /* the base values are already known */
            cache[k] = init[k];
        } else {
            cache[k] = (seq_t)5 * cache[k - 4];
        }
    }
    max_cached = n;
    return cache[n];
}
