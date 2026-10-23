/*
 * A028455  Number of perfect matchings in graph P_{2} X C_{3} X P_{n}.
 * https://oeis.org/A028455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1)+22*a(n-2)-60*a(n-3)-67*a(n-4) *
 *            +60*a(n-5)+22*a(n-6)-8*a(n-7)-a(n-8) for n ≥ 8 * with initial
 *            values a(0)=1, a(1)=4, a(2)=50, a(3)=444, * a(4)=4349,
 *            a(5)=41348, a(6)=396733, a(7)=3795912
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static const seq_t base_vals[8] = {
    (seq_t)1UL,
    (seq_t)4UL,
    (seq_t)50UL,
    (seq_t)444UL,
    (seq_t)4349UL,
    (seq_t)41348UL,
    (seq_t)396733UL,
    (seq_t)3795912UL
};

static seq_t *cache = NULL;          /* dynamic array of computed values   */
static idx_t  cache_size = 0;        /* allocated size of cache           */
static idx_t  next_filled = 0;       /* first index that is not yet set   */

static seq_t *
resize_cache(idx_t new_size)
{
#if defined(__cplusplus)
    seq_t *new_cache = new seq_t[new_size];
    if (cache != NULL) {
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
    }
    return new_cache;
#else
    return (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* Ensure we have space for n */
    if (n + 1 > cache_size) {
        idx_t new_cap = n + 1;
        /* grow a bit to reduce reallocs */
        if (new_cap < 8) new_cap = 8;
        new_cap = (new_cap > cache_size * 2 ? new_cap : cache_size * 2);
        cache = resize_cache(new_cap);
        cache_size = new_cap;
    }

    /* Fill base values if not yet stored */
    if (next_filled < 8) {
        idx_t upto = next_filled < 8 ? 8 : next_filled;
        if (upto > cache_size) upto = cache_size;
        for (idx_t i = next_filled; i < upto; ++i)
            cache[i] = base_vals[i];
        next_filled = upto;
    }

    /* Compute missing values up to n */
    for (idx_t i = next_filled; i <= n; ++i) {
        /* recurrence requires i>=8, base values already ensured */
        seq_t v =  (seq_t)8 * cache[i - 1]
                 + (seq_t)22 * cache[i - 2]
                 - (seq_t)60 * cache[i - 3]
                 - (seq_t)67 * cache[i - 4]
                 + (seq_t)60 * cache[i - 5]
                 + (seq_t)22 * cache[i - 6]
                 - (seq_t)8  * cache[i - 7]
                 -        cache[i - 8];
        cache[i] = v;
    }
    next_filled = n + 1;
    return cache[n];
}
