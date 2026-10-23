/*
 * A041147  Denominators of continued fraction convergents to sqrt(83).
 * https://oeis.org/A041147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Recurrence: a(0)=1, a(1)=9, a(2)=163, a(3)=1476,
 * a(n) = 164*a(n-2) - a(n-4)   (n >= 4)
 */
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Ensure the cache can hold up to n */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize first four terms if not yet done */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        cache[2] = (seq_t)163;
        cache[3] = (seq_t)1476;
        cache_size = 4;
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 4) continue;  /* already initialized */
        cache[i] = (seq_t)164 * cache[i-2] - cache[i-4];
    }
    cache_size = n + 1;

    return cache[n];
}
