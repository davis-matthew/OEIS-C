/*
 * A147851  Expansion of 1/(1 - x^3 - x^4 - x^5 + x^8)^2.
 * https://oeis.org/A147851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-3) + 2*a(n-4) + 2*a(n-5) - a(n-6) - 2*a(n-7)
 *            -5*a(n-8) - 2*a(n-9) - a(n-10) + 2*a(n-11) + 2*a(n-12) +
 *            2*a(n-13) - a(n-16); a(0)=1, a(1)=0, a(2)=0, a(3)=2, a(4)=2,
 *            a(5)=2, a(6)=3, a(7)=6, a(8)=7, a(9)=10, a(10)=15, a(11)=18,
 *            a(12)=27, a(13)=38, a(14)=50, a(15)=66
 * Range:     u8 n <= 19, u16 n <= 39, u32 n <= 81, u64 n <= 168,
 *            u128 n <= 344, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: memoised recurrence */
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t computed_max = 0; /* highest index whose value is stored */

static const seq_t init_vals[16] = {
    1, 0, 0, 2, 2, 2, 3, 6, 7, 10, 15, 18, 27, 38, 50, 66
};

seq_t a(idx_t n)
{
    /* Ensure the cache is large enough */
    if (n >= cache_cap) {
        size_t old_cap = cache_cap;
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
        /* Initialise new entries */
        for (size_t i = old_cap; i < new_cap; ++i) {
            if (i < 16)
                cache[i] = init_vals[i];
            else
                cache[i] = 0;          /* for indices not yet computed */
        }
    }

    /* If the requested value is already computed, return it */
    if ((size_t)n <= computed_max)
        return cache[(size_t)n];

    /* Compute missing values up to n */
    for (size_t i = computed_max + 1; i <= (size_t)n; ++i) {
        seq_t val;
        if (i < 16)
            val = init_vals[i];
        else
            val = 2*cache[i-3] + 2*cache[i-4] + 2*cache[i-5]
                  - cache[i-6] - 2*cache[i-7] - 5*cache[i-8]
                  - 2*cache[i-9] - cache[i-10]
                  + 2*cache[i-11] + 2*cache[i-12] + 2*cache[i-13]
                  - cache[i-16];
        cache[i] = val;
    }

    computed_max = (size_t)n;
    return cache[(size_t)n];
}
