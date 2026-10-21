/*
 * A208370  Number of 4 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 0 1 0 vertically.
 * https://oeis.org/A208370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 19*a(n-2) + 23*a(n-3) - 26*a(n-4) - 38*a(n-5) +
 *            13*a(n-6) + 15*a(n-7) - 4*a(n-8); Initial values: a(1)=9,
 *            a(2)=81, a(3)=351, a(4)=1953, a(5)=9999, a(6)=52821,
 *            a(7)=275769, a(8)=1446381
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed values, index 0 unused */
static size_t cache_cap = 0;     /* current allocated size */
static idx_t cache_len = 0;      /* first index not yet computed */

/* explicit base values for indices 1..8, index 0 unused */
static const seq_t base_vals[9] = {
    0ULL,                     /* unused */
    9ULL, 81ULL, 351ULL, 1953ULL,
    9999ULL, 52821ULL, 275769ULL,
    1446381ULL
};

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* sequence is defined for n>=1 */

    /* if already computed, return it directly */
    if (n < cache_len)
        return cache[n];

    /* determine how much storage is needed (at least up to n, plus index 0) */
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) {               /* allocation failure – return 0 as a safe fallback */
            return 0;
        }
        cache = tmp;
        cache_cap = needed;
    }

    /* compute all missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i <= 8) {
            cache[i] = base_vals[i];
        } else {
            cache[i] = cache[i-1]
                     + 19 * cache[i-2]
                     + 23 * cache[i-3]
                     - 26 * cache[i-4]
                     - 38 * cache[i-5]
                     + 13 * cache[i-6]
                     + 15 * cache[i-7]
                     -  4 * cache[i-8];
        }
    }
    cache_len = n + 1;          /* next index that is not yet computed */
    return cache[n];
}
