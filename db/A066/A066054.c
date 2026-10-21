/*
 * A066054  'Reverse and Add!' trajectory of 10583.
 * https://oeis.org/A066054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(0) = 10583; for n ≥ 1, * a(n) = a(n-1) +
 *            reverse_digits(a(n-1)) (decimal reverse)
 * Range:     u8 none, u16 n <= 1, u32 n <= 11, u64 n <= 34, u128 n <= 81,
 *            big any n
 * Helpers:   rev_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/rev_digits.h"

/* Cache of computed terms */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of elements stored (next index to compute) */
static idx_t cache_cap = 0;   /* allocated size */

static void grow_cache(idx_t needed)
{
    if (needed < cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap <= needed)
        new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    cache = new_ptr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (cache_len == 0) {
        /* initialise with the seed value */
        grow_cache(0);
        cache[0] = (seq_t)10583;
        cache_len = 1;
    }

    if (n < cache_len)
        return cache[n];

    /* compute missing terms up to n */
    grow_cache(n);
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev = cache[i-1];
        seq_t rev = rev_digits(prev);
        cache[i] = prev + rev;
    }
    cache_len = n + 1;

    return cache[n];
}
