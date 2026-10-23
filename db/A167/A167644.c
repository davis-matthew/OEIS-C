/*
 * A167644  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 * https://oeis.org/A167644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167644
 * Number of reduced words of length n in the Coxeter group on 47 generators
 * S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 *
 * The generating function gives the linear recurrence
 *     a(n) = 45·(a(n-1)+…+a(n-13)) – 1035·a(n-14) + e(n)
 * with the inhomogeneous term
 *     e(0)=1,  e(1)=…=e(13)=2,  e(14)=1,  e(n)=0 for n>14.
 * The first 14 values (a(0)…a(13)) are taken from the OEIS entry and
 * are used to initialise the cache.
 *
 * This implementation stores the values in a dynamically growing
 * static cache so that calls can be made in any order.
 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"   /* provides seq_t seq_from_str(const char *) */

static const char * const init_str[14] = {
    "1",
    "47",
    "2162",
    "99452",
    "4574792",
    "210440432",
    "9680259872",
    "445291954112",
    "20483429889152",
    "942237774900992",
    "43342937645445632",
    "1993775131690499072",
    "91713656057762957312",
    "4218828178657096036352"
};

static bool init_done   = false;
static seq_t init_vals[14];

/*
 * a(idx_t n)
 * Computes the nth term of sequence A167644.
 * Cache the results so that repeated calls with any ordering of n
 * are efficient.
 */
seq_t a(idx_t n)
{
    /* parse the initial values once */
    if (!init_done) {
        for (idx_t i = 0; i < 14; ++i)
            init_vals[i] = seq_from_str(init_str[i]);
        init_done = true;
    }

    /* static cache of already computed values */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;    /* number of values stored */
    static idx_t cache_cap   = 0;    /* allocated capacity */

    /* initialise cache with capacity for at least 14 terms */
    if (!cache) {
        cache_cap = 32;
        if (cache_cap < 14)
            cache_cap = 14;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i < 14; ++i)
            cache[i] = init_vals[i];
        cache_size = 14;
    }

    /* extend cache if needed */
    if (n >= cache_size) {
        idx_t needed = n + 1;
        if (needed > cache_cap) {
            idx_t new_cap = cache_cap * 2;
            while (new_cap < needed)
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();   /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }
        for (idx_t i = cache_size; i <= n; ++i) {
            /* recurrence implementation for i ≥ 14 */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 13; ++k)
                sum += cache[i - k];
            if (i == 14) {
                /* add the inhomogeneous term e(14) = 1 */
                cache[i] = 45 * sum - 1035 * cache[i - 14] + (seq_t)1;
            } else { /* i > 14, e(i) = 0 */
                cache[i] = 45 * sum - 1035 * cache[i - 14];
            }
        }
        cache_size = n + 1;
    }

    return cache[n];
}
