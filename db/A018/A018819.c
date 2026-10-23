/*
 * A018819  Binary partition function: number of partitions of n into powers
 *          of 2.
 * https://oeis.org/A018819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(0)=1; for n>0, a(2m+1)=a(2m), a(2m)=a(2m-1)+a(m). This
 *            computes the binary partition function (OEIS A018819)
 * Range:     u8 n <= 35, u16 n <= 157, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;
static idx_t cache_size = 0;

/* Compute values up to index n inclusive, extending the cache if needed. */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size)
        return;

    idx_t new_size = n + 1;
    seq_t *tmp;
    if (a_cache == NULL) {
        tmp = (seq_t*)malloc(new_size * sizeof(seq_t));
    } else {
        tmp = (seq_t*)realloc(a_cache, new_size * sizeof(seq_t));
    }
    a_cache = tmp;

    /* Initialise the first entry if it hasn't been set yet. */
    if (cache_size == 0) {
        a_cache[0] = (seq_t)1;
    }

    /* Compute new values. */
    idx_t start = cache_size > 0 ? cache_size : 1;
    for (idx_t i = start; i <= n; ++i) {
        if (i & 1) {           /* odd index: a(i) = a(i-1) */
            a_cache[i] = a_cache[i - 1];
        } else {               /* even index: a(i) = a(i-1) + a(i/2) */
            a_cache[i] = a_cache[i - 1] + a_cache[i / 2];
        }
    }

    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}
