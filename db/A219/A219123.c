/*
 * A219123  Number of nX3 arrays of the minimum value of corresponding
 *          elements and their horizontal, diagonal or antidiagonal neighbors
 *          in a random 0..2 nX3 array.
 * https://oeis.org/A219123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) -5*a(n-2) -24*a(n-3) -70*a(n-4) -58*a(n-5)
 *            -81*a(n-6) * -200*a(n-7) +269*a(n-8) +1056*a(n-9) +1650*a(n-10)
 *            +1036*a(n-11) +200*a(n-12) * Offset: 1 (a(1) = 9)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static const seq_t init[13] = {
    (seq_t)0,                          /* dummy for n = 0 */
    (seq_t)9,
    (seq_t)87,
    (seq_t)787,
    (seq_t)6981,
    (seq_t)63183,
    (seq_t)571149,
    (seq_t)5167013,
    (seq_t)46755215,
    (seq_t)423105613,
    (seq_t)3828940359ULL,
    (seq_t)34650738731ULL,
    (seq_t)313579561365ULL
};

static seq_t *cache = NULL;   /* cache[0] unused; cache[1] .. cache[n] valid */
static size_t cache_size = 0;  /* number of valid entries currently allocated */

static void ensure_cache(idx_t n)
{
    /* Make sure we have storage for indices 0 .. n inclusive. */
    if (cache_size < (size_t)(n + 1)) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) { /* allocation failed – abort */
            exit(EXIT_FAILURE);
        }
        /* initialise new entries to zero */
        for (size_t i = cache_size; i < (size_t)(n + 1); ++i) {
            tmp[i] = (seq_t)0;
        }
        cache = tmp;
        cache_size = (size_t)(n + 1);
    }

    /* initialise the first 12 terms if they are not already set */
    for (idx_t i = 1; i <= 12 && i <= n; ++i) {
        if (cache[(size_t)i] == (seq_t)0) {
            cache[(size_t)i] = init[i];
        }
    }

    /* compute missing values using the recurrence */
    for (idx_t i = 13; i <= n; ++i) {
        if (cache[(size_t)i] == (seq_t)0) {
            seq_t pos =
                cache[(size_t)(i - 1)] * (seq_t)10
              + cache[(size_t)(i - 8)] * (seq_t)269
              + cache[(size_t)(i - 9)] * (seq_t)1056
              + cache[(size_t)(i - 10)] * (seq_t)1650
              + cache[(size_t)(i - 11)] * (seq_t)1036
              + cache[(size_t)(i - 12)] * (seq_t)200;
            seq_t neg =
                cache[(size_t)(i - 2)] * (seq_t)5
              + cache[(size_t)(i - 3)] * (seq_t)24
              + cache[(size_t)(i - 4)] * (seq_t)70
              + cache[(size_t)(i - 5)] * (seq_t)58
              + cache[(size_t)(i - 6)] * (seq_t)81
              + cache[(size_t)(i - 7)] * (seq_t)200;
            cache[(size_t)i] = pos - neg;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0; /* undefined for offset 1, return 0 */
    }
    ensure_cache(n);
    return cache[(size_t)n];
}
