/*
 * A008031  Coordination sequence T1 for Zeolite Code ANA.
 * https://oeis.org/A008031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 165, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coord. Seq. T1 for Zeolite ANA (OEIS A008031)
   a(0) = 1, a(1) = 4, a(2) = 10, a(3) = 22, a(4) = 39,
   a(5) = 60, a(6) = 87, a(7) = 118, a(8) = 154, a(9) = 196,
   a(10) = 242, a(11) = 292, a(12) = 345, a(13) = 406,
   a(14) = 471, a(15) = 540, a(16) = 616.
   For n ≥ 17:
       a(n) = a(n-1) + a(n-5) - a(n-6) + a(n-8)
              - a(n-9) - a(n-13) + a(n-14)  */
#include <stdlib.h>

/* First 17 terms */
static const seq_t init[17] = {
    1, 4, 10, 22, 39, 60, 87,
    118, 154, 196, 242, 292, 345,
    406, 471, 540, 616
};

/* Cached values (lazily allocated).  cache_cap holds allocated size,
   computed_n is the greatest index for which a value is stored. */
static seq_t *cache      = NULL;
static size_t  cache_cap = 0;
static idx_t   computed_n = 16;   /* last index with a value, 16 is valid */

/* ------------------------------------------------------------------ */
/* Compute a(n) for any n ≥ 0                                             */
seq_t a(idx_t n)
{
    /* For n < 17 we return the pre‑computed constant */
    if (n < 17)
        return init[(size_t)n];

    /* Ensure we have a cache at least large enough for indices 0..n */
    size_t needed = (size_t)n + 1;
    if (!cache || cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap : 17;
        while (new_cap < needed)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
        if (!cache_cap) return 0;    /* shouldn't happen */

        if (cache_cap >= 17) {
            for (size_t i = 0; i < 17; ++i)
                cache[i] = init[i];
        }
    }

    /* Compute missing terms only */
    idx_t start = computed_n + 1;
    if (start <= 16)               /* first computable index is 17 */
        start = 17;
    for (idx_t i = start; i <= n; ++i) {
        /* a(i) = a(i-1) + a(i-5) - a(i-6) + a(i-8)
                 - a(i-9) - a(i-13) + a(i-14) */
        seq_t pos = cache[i - 1] + cache[i - 5] + cache[i - 8] + cache[i - 14];
        seq_t neg = cache[i - 6] + cache[i - 9] + cache[i - 13];
        cache[i] = pos - neg;
    }
    computed_n = n;
    return cache[(size_t)n];
}
