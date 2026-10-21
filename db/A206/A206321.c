/*
 * A206321  Number of (n+1) X 3 0..3 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly three clockwise edge increases.
 * https://oeis.org/A206321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24*a(n-1) -131*a(n-2) +200*a(n-3) -116*a(n-4) +35*a(n-5)
 *            -13*a(n-6) +2*a(n-7) for n>10; Initial terms: a(1)=1344,
 *            a(2)=16892, a(3)=282996, a(4)=4796092, a(5)=81302880,
 *            a(6)=1377662868, a(7)=23340152044, a(8)=395403752128,
 *            a(9)=6698395782004, a(10)=113474612575628
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_terms[10] = {
    (seq_t)1344ull,
    (seq_t)16892ull,
    (seq_t)282996ull,
    (seq_t)4796092ull,
    (seq_t)81302880ull,
    (seq_t)1377662868ull,
    (seq_t)23340152044ull,
    (seq_t)395403752128ull,
    (seq_t)6698395782004ull,
    (seq_t)113474612575628ull
};

static seq_t *cache = NULL;           /* cache[0] unused, cache[1] = a(1) ... */
static idx_t cache_size = 0;          /* current allocated size (>= 1) */

static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size)
        return;

    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed – abort the program. */
        abort();
    }
    /* Initialize newly allocated entries to zero. */
    for (idx_t i = cache_size; i < new_size; ++i)
        new_cache[i] = (seq_t)0;
    cache = new_cache;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for offset, safe return */

    ensure_cache(n);

    /* Load base terms if not already present. */
    if (n >= 1) {
        for (idx_t i = 1; i <= 10 && i <= n; ++i) {
            if (cache[i] == (seq_t)0)
                cache[i] = init_terms[i - 1];
        }
    }

    /* Compute terms >10 using the recurrence. */
    for (idx_t i = 11; i <= n; ++i) {
        if (cache[i] == (seq_t)0) {
            cache[i] = (seq_t)24  * cache[i - 1]
                     - (seq_t)131 * cache[i - 2]
                     + (seq_t)200 * cache[i - 3]
                     - (seq_t)116 * cache[i - 4]
                     + (seq_t)35  * cache[i - 5]
                     - (seq_t)13  * cache[i - 6]
                     + (seq_t)2   * cache[i - 7];
        }
    }

    return cache[n];
}
