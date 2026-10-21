/*
 * A219113  Sequence of integers which are simultaneously a sum of consecutive
 *          squares and a difference of consecutive cubes.
 * https://oeis.org/A219113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 98*a(n-1) - a(n-2) - 36; a(1) = 1, a(2) = 61 */
#include <stdlib.h>

static seq_t *cache = NULL;        /* cache[0] == a(1), cache[1] == a(2), ... */
static idx_t  last_computed = 0;   /* largest n whose value is stored */
static size_t cache_cap = 0;       /* allocated size of cache array */

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1 – value for n==0 never used */
        return (seq_t)0;

    /* Ensure cache has room for n elements (indices 0 … n-1) */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap : 1;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise base values if necessary */
    if (last_computed < 1) {
        cache[0] = (seq_t)1;        /* a(1) */
        last_computed = 1;
        if (n == 1) return cache[0];
    }

    if (last_computed < 2) {
        cache[1] = (seq_t)61;       /* a(2) */
        last_computed = 2;
        if (n == 2) return cache[1];
    }

    /* Compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* i >= 3, recurrence uses a(i-1), a(i-2) */
        cache[i-1] = (seq_t)98 * cache[i-2] - cache[i-3] - (seq_t)36;
    }
    last_computed = n;
    return cache[n-1];
}
