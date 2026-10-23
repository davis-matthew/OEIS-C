/*
 * A180003  Place a(n) blue and b(n) (A180002) red balls in an urn, draw 5
 *          balls without replacement; Probability(5 red balls) =
 *          Probability(3 red and 2 blue balls).
 * https://oeis.org/A180003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 39*a(n-3) - 39*a(n-6) + a(n-9) a(1)=1, a(2)=2, a(3)=7,
 *            a(4)=13, a(5)=56, a(6)=247, a(7)=475, a(8)=2108, a(9)=9361
 *            (trivial recurrence with offset 1)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code uses memoisation to compute the sequence quickly. */
#include <stdlib.h>

/* The external types seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* Static cache for memoisation.  cache[0] is unused. */
    static seq_t* cache = NULL;
    static size_t cache_len = 0;   /* highest index stored in cache */

    /* Static initial values for indices 1..9. */
    static const seq_t init[10] = {
        0ULL,          /* 0 unused */
        (seq_t)1,      /* a(1) */
        (seq_t)2,      /* a(2) */
        (seq_t)7,      /* a(3) */
        (seq_t)13,     /* a(4) */
        (seq_t)56,     /* a(5) */
        (seq_t)247,    /* a(6) */
        (seq_t)475,    /* a(7) */
        (seq_t)2108,   /* a(8) */
        (seq_t)9361    /* a(9) */
    };
    const seq_t K = (seq_t)39;   /* factor used in the recurrence */

    /* Helper: extend the cache up to at least index n. */
    if (cache_len < (size_t)n) {
        size_t new_len = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, (new_len + 1) * sizeof(seq_t));
        if (!new_cache) abort(); /* memory exhaustion, stop execution */
        cache = new_cache;
        for (size_t i = cache_len + 1; i <= new_len; ++i) {
            if (i <= 9) {
                cache[i] = init[(size_t)i];
            } else {
                seq_t t1 = K * cache[i - 3];
                seq_t t2 = K * cache[i - 6];
                cache[i] = t1 - t2 + cache[i - 9];
            }
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
