/*
 * A117903  Diagonal sums of number triangle A117901.
 * https://oeis.org/A117903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -a(n-1) -a(n-2) +3*a(n-3) +3*a(n-4) +3*a(n-5) +4*a(n-6)
 *            +4*a(n-7) +4*a(n-8) Initial values: a(0)=1, a(1)=-1, a(2)=1,
 *            a(3)=-2, a(4)=4, a(5)=-2, a(6)=-5, a(7)=14
 * Range:     i8 n <= 12, i16 n <= 24, i32 n <= 48, i64 n <= 96, i128 n <= 192,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t INIT[8] = { (seq_t)1, (seq_t)-1, (seq_t)1, (seq_t)-2,
                               (seq_t)4, (seq_t)-2, (seq_t)-5, (seq_t)14 };

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Allocate initial cache if not yet done */
    if (!cache) {
        cache_size = 8;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < 8; ++i)
            cache[i] = INIT[i];
    }

    /* Grow cache if needed */
    if (n >= cache_size) {
        idx_t needed = n + 1;
        idx_t new_size = cache_size;
        while (new_size < needed)
            new_size <<= 1;          /* double until large enough */

        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed; fall back to recomputing from scratch */
            free(cache);
            cache = NULL;
            return a(n);            /* will reallocate */
        }
        cache = new_cache;
        cache_size = new_size;
    }

    /* If the value is already computed, return it */
    if (n < cache_size && n < (idx_t)8)
        return cache[n];

    /* Compute values up to n using the recurrence */
    for (idx_t i = 8; i <= n; ++i) {
        seq_t val = -(cache[i - 1]) - (cache[i - 2])
                    + (seq_t)3 * cache[i - 3] + (seq_t)3 * cache[i - 4]
                    + (seq_t)3 * cache[i - 5] + (seq_t)4 * cache[i - 6]
                    + (seq_t)4 * cache[i - 7] + (seq_t)4 * cache[i - 8];
        cache[i] = val;
    }

    return cache[n];
}
