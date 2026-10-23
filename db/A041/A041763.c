/*
 * A041763  Denominators of continued fraction convergents to sqrt(402).
 * https://oeis.org/A041763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, less
 * Formula:   a(n) = 802*a(n-2) - a(n-4); a(0)=1, a(1)=20, a(2)=801,
 *            a(3)=16040
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed values */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t max_computed = 0;  /* highest index that has been computed */

/* Ensure that cache has space for at least n+1 elements and that a(0..max(n,3))
   are computed. */
static void cache_ensure(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the first four elements once */
    if (max_computed < 4) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)20;
        cache[2] = (seq_t)801;
        cache[3] = (seq_t)16040;
        max_computed = 3;
    }

    /* compute further terms up to n */
    size_t i = max_computed + 1;
    while (i <= (size_t)n) {
        cache[i] = (802 * cache[i - 2]) - cache[i - 4];
        ++i;
    }
    if (i - 1 > max_computed) max_computed = i - 1;
}

seq_t a(idx_t n)
{
    cache_ensure(n);
    return cache[(size_t)n];
}
