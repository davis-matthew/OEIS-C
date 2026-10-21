/*
 * A060995  Number of routes of length 2n on the sides of an octagon from a
 *          point to opposite point.
 * https://oeis.org/A060995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2); a(1)=0, a(2)=2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *seq_cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_cap   = 0;

/* ensure sufficient capacity for the cache */
static void ensure_cache_capacity(idx_t need)
{
    if (need <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap : 2;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_ptr) abort();
    if (seq_cache) {
        for (idx_t i = 0; i < cache_size; ++i) new_ptr[i] = seq_cache[i];
        free(seq_cache);
    }
    seq_cache = new_ptr;
    cache_cap = new_cap;
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;

    ensure_cache_capacity(n);

    /* initialise base terms if not yet done */
    if (cache_size == 0) {
        seq_cache[0] = (seq_t)0;
        seq_cache[1] = (seq_t)2;
        cache_size = 2;
    }

    /* compute missing terms up to n */
    while (cache_size < n) {
        seq_t next = seq_cache[cache_size - 1] * (seq_t)4;
        next -= seq_cache[cache_size - 2] * (seq_t)2;
        seq_cache[cache_size++] = next;
    }

    return seq_cache[n - 1];
}
