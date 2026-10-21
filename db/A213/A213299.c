/*
 * A213299  Partial sums of A211681.
 * https://oeis.org/A213299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 9*a(n-4) - 18*a(n-5) + 9*a(n-6)
 *            +10*a(n-8) -20*a(n-9) +10*a(n-10) for n>10 Base values: a(1)=2,
 *            a(2)=5, a(3)=10, a(4)=17, a(5)=40, a(6)=77, a(7)=130, a(8)=203,
 *            a(9)=440, a(10)=813
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 36, u64 n <= 75, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache        = NULL; /* cached sequence values (index 0 unused) */
static size_t cache_cap    = 0;    /* allocated capacity (indices 0..cache_cap-1) */
static idx_t cache_len     = 0;    /* highest index that has been computed */

static const seq_t BASE_VALUES[11] = {
    0,          /* a(0) unused */
    2, 5, 10, 17, 40,
    77, 130, 203, 440, 813
};

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safety guard – sequence is defined for n ≥ 1 */

    /* Ensure the cache is large enough for index n */
    if (!cache || cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache = tmp;
        cache_cap = new_cap;
        if (!cache) return 0; /* allocation failure – outside the correct range */
    }

    /* Compute all missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 10) { /* base values */
            cache[(size_t)i] = BASE_VALUES[(size_t)i];
        } else { /* recurrence for i > 10 */
            seq_t val =
                2 * cache[(size_t)(i-1)]
              - cache[(size_t)(i-2)]
              + 9 * cache[(size_t)(i-4)]
              - 18 * cache[(size_t)(i-5)]
              + 9 * cache[(size_t)(i-6)]
              + 10 * cache[(size_t)(i-8)]
              - 20 * cache[(size_t)(i-9)]
              + 10 * cache[(size_t)(i-10)];
            cache[(size_t)i] = val;
        }
        if (i > cache_len) cache_len = i;
    }

    return cache[(size_t)n];
}
