/*
 * A208266  Number of nX5 0..1 arrays with new values 0..1 introduced in row
 *          major order and no element equal to more than two of its immediate
 *          leftward or upward or left-upward diagonal neighbors.
 * https://oeis.org/A208266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 21*a(n-1) + 9*a(n-2) - 278*a(n-3) + 73*a(n-4) * +
 *            790*a(n-5) - 662*a(n-6) + 29*a(n-7) + 69*a(n-8) - 10*a(n-9) *
 *            Initial values: a(1)=16, a(2)=317, a(3)=6631, a(4)=137845, *
 *            a(5)=2867739, a(6)=59655167, a(7)=1240971177, *
 *            a(8)=25815151595, a(9)=537016650729
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* external integer types supplied by the build */
extern seq_t a(idx_t n);

static seq_t *cache = NULL;        /* 0‑based array storing a(1) in cache[0] */
static idx_t  cache_capacity = 0;  /* current allocated size of cache */
static idx_t  max_computed   = 0;  /* largest n for which a(n) is known */

static const seq_t init_values[9] = {
    (seq_t)16ULL,          (seq_t)317ULL,            (seq_t)6631ULL,
    (seq_t)137845ULL,      (seq_t)2867739ULL,        (seq_t)59655167ULL,
    (seq_t)1240971177ULL,  (seq_t)25815151595ULL,   (seq_t)537016650729ULL
};

/* Ensure the cache has room for at least n entries (1‑based). */
static void ensure_cache(idx_t n)
{
    if (n <= cache_capacity)
        return;

    /* Grow cache; use a factor of 2 or a bit more for the first allocation. */
    size_t new_cap = (size_t)n * 2;
    if (new_cap == 0)          /* when n == 0, protect against overflow */
        new_cap = 16;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1); /* out of memory */

    /* Initialize new slots to zero (sentinel for uncomputed values). */
    if (new_cap > cache_capacity)
        for (size_t i = cache_capacity; i < new_cap; ++i)
            tmp[i] = 0;

    cache = tmp;
    cache_capacity = (idx_t)new_cap;
}

/* Return a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n < 1 */

    ensure_cache(n);

    if (max_computed < n)
    {
        for (idx_t idx = max_computed + 1; idx <= n; ++idx)
        {
            if (idx <= 9)
            {
                /* Use stored initial values. */
                cache[idx - 1] = init_values[idx - 1];
            }
            else
            {
                /* Recurrence for n >= 10.  All required a(k) for k < n are
                 * already stored in cache at positions k-1. */
                seq_t pos = 21 * cache[idx - 2]
                          + 9  * cache[idx - 3]
                          + 73 * cache[idx - 5]
                          + 790* cache[idx - 6]
                          + 29 * cache[idx - 8]
                          + 69 * cache[idx - 9];

                seq_t neg = 278* cache[idx - 4]
                          + 662* cache[idx - 7]
                          + 10 * cache[idx -10];

                cache[idx - 1] = pos - neg;
            }
        }
        max_computed = n;
    }

    return cache[n - 1];
}
