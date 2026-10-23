/*
 * A159612  INVERT transform of (1, 3, 1, 3, 1, ...).
 * https://oeis.org/A159612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2), with a(1)=1, a(2)=4
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;           /* cache[1..] holds the sequence values   */
static idx_t cache_len = 0;           /* highest index already computed        */
static idx_t cache_cap = 0;           /* current allocated capacity (>= cache_len) */

static void ensure_capacity(idx_t req)
{
    if (cache_cap < req + 1) {        /* +1 for unused index 0                */
        idx_t new_cap = cache_cap ? cache_cap << 1 : 2;
        while (new_cap < req + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return a(n) for any n >= 1; memoised so earlier values are cached. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n < 1, but defined as 0   */

    /* initialise cache if necessary */
    if (!cache) {
        cache = (seq_t *)calloc(3, sizeof(seq_t)); /* indices 0..2     */
        cache_cap = 3;
        cache_len = 0;
    }

    if (n > cache_len) {
        ensure_capacity(n);
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i == 1) {
                cache[i] = (seq_t)1;
            } else if (i == 2) {
                cache[i] = (seq_t)4;
            } else {
                cache[i] = cache[i-1] + 4 * cache[i-2];
            }
        }
        cache_len = n;
    }
    return cache[n];
}
