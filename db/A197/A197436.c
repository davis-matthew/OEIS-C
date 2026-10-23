/*
 * A197436  Number of nX2 0..2 arrays with no element equal to the average of
 *          its horizontal and vertical neighbors.
 * https://oeis.org/A197436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   mul_small, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical Formula: 
   a(n) = 7*a(n-1) + 6*a(n-2) - 25*a(n-3) - 57*a(n-4) - 7*a(n-5)
          + 78*a(n-6) + 71*a(n-7) + 6*a(n-8) - 18*a(n-9) - 10*a(n-10) - 8*a(n-11)
   Initial values: a(1)=6,a(2)=34,a(3)=246,a(4)=1736,a(5)=12450,
                   a(6)=89528,a(7)=644328,a(8)=4638654,a(9)=33398106,
                   a(10)=240473126,a(11)=1731475912
*/

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mul_small.h"
#include "../utils/seq_realloc.h"

static const seq_t init_vals[11] = {
    (seq_t)6,
    (seq_t)34,
    (seq_t)246,
    (seq_t)1736,
    (seq_t)12450,
    (seq_t)89528,
    (seq_t)644328,
    (seq_t)4638654,
    (seq_t)33398106,
    (seq_t)240473126,
    (seq_t)1731475912
};

static seq_t *cache = NULL;     /* cache[0] unused */
static size_t cache_cap = 0;    /* number of allocated slots (0..cache_cap-1) */

static void grow_cache(idx_t n)
{
    /* Ensure cache has slots 0..n. */
    if (cache_cap > (size_t)n)
        return;

    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    if (!tmp)
        abort();                 /* memory failure */

    cache = tmp;
    /* initialise new slots */
    for (size_t i = cache_cap; i < new_cap; ++i) {
        if (i == 0)
            cache[i] = 0;        /* unused */
        else if (i <= 11)
            cache[i] = init_vals[i - 1];
        else
            cache[i] = 0;
    }
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        abort();                 /* out of bounds */

    if (n <= 11)
        return init_vals[(size_t)n - 1];

    size_t old_cap = cache_cap;
    grow_cache(n);

    size_t start = old_cap;
    if (start < 12)
        start = 12;

    for (size_t i = start; i <= (size_t)n; ++i) {
        seq_t pos = mul_small(cache[i - 1], 7);
        pos += mul_small(cache[i - 2], 6);
        pos += mul_small(cache[i - 6], 78);
        pos += mul_small(cache[i - 7], 71);
        pos += mul_small(cache[i - 8], 6);

        seq_t neg = mul_small(cache[i - 3], 25);
        neg += mul_small(cache[i - 4], 57);
        neg += mul_small(cache[i - 5], 7);
        neg += mul_small(cache[i - 9], 18);
        neg += mul_small(cache[i - 10], 10);
        neg += mul_small(cache[i - 11], 8);

        cache[i] = pos - neg;
    }

    return cache[(size_t)n];
}
