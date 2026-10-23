/*
 * A172319  10th column of A172119.
 * https://oeis.org/A172319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+10) = 2*a(n+9) – a(n) a(0) = 1, a(1) = 2, …, a(9) = 512
 *            Computes a(n) for any n ≥ 0 using memoisation. The recursion
 *            holds for all n ≥ 10
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoised values */
static size_t cache_len = 0;  /* number of computed entries */
static size_t cache_cap = 0;  /* allocated capacity in seq_t units */

seq_t a(idx_t n)
{
    /* initialise cache if required */
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* grow cache until it contains at least index n */
    while (cache_len <= n) {
        /* ensure capacity for the new element */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        if (cache_len == 0) {
            cache[0] = (seq_t)1;
        } else if (cache_len < 10) {
            cache[cache_len] = cache[cache_len - 1] * (seq_t)2;
        } else {
            seq_t twice = cache[cache_len - 1] * (seq_t)2;
            cache[cache_len] = twice - cache[cache_len - 10];
        }
        cache_len++;
    }

    return cache[n];
}
