/*
 * A036557  Number of multiples of 3 in 0..2^n-1 with an even sum of base-2
 *          digits.
 * https://oeis.org/A036557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) - 8*a(n-3) - 3*a(n-4) + 6*a(n-5) with
 *            initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=6, a(5)=10
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[6] = {1, 1, 2, 3, 6, 10};

static seq_t *cache = NULL;     /* dynamic cache of computed values */
static size_t cache_size = 0;   /* allocated size of cache */
static idx_t  last_comp = -1;   /* largest n for which cache[n] is valid */

seq_t a(idx_t n)
{
    /* quick return for the first six values */
    if (n < 6)
        return init_vals[n];

    /* initial cache allocation (ensure at least 6 entries) */
    if (!cache) {
        cache_size = (size_t)n + 1;
        if (cache_size < 6)
            cache_size = 6;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure */
        for (size_t i = 0; i < 6; ++i)
            cache[i] = init_vals[i];
        last_comp = 5;
    }

    /* grow the cache if necessary */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* realloc failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing values up to n using the recurrence */
    while (last_comp < n) {
        idx_t i = last_comp + 1;
        seq_t val = cache[i-1] * (seq_t)2;
        val += cache[i-2] * (seq_t)4;
        val -= cache[i-3] * (seq_t)8;
        val -= cache[i-4] * (seq_t)3;
        val += cache[i-5] * (seq_t)6;
        cache[i] = val;
        ++last_comp;
    }

    return cache[n];
}
