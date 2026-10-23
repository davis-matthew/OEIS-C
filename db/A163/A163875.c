/*
 * A163875  a(n)=n-a(a(n-4)) with a(0)=a(1)=a(2)=a(3)=0.
 * https://oeis.org/A163875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - a(a(n-4)), a(0)=a(1)=a(2)=a(3)=0
 * Range:     u8 n <= 411, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Cached sequence values */
static seq_t *cache = NULL;
/* Current allocation size (number of elements stored) */
static size_t cache_cap = 0;
/* Largest index that has been computed */
static idx_t computed_upto = (idx_t)-1;

/* Ensure cache is large enough for indices up to n and
   compute any missing values in order. */
static void ensure_computed(idx_t n)
{
    /* Allocate and initialise cache if this is the first call */
    if (cache == NULL) {
        size_t init_cap = (n >= 8) ? n + 1 : 16;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);
        cache_cap = init_cap;
        /* initialise first four values */
        cache[0] = cache[1] = cache[2] = cache[3] = 0;
        computed_upto = 3;
    }

    /* Grow cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing values up to n */
    idx_t start = (computed_upto < 4) ? 4 : computed_upto + 1;
    for (idx_t i = start; i <= n; ++i) {
        /* cache[i-4] is guaranteed to be computed */
        seq_t before = cache[i - 4];
        idx_t sub = (idx_t)before;   /* safe: before <= i-4 */
        cache[i] = (seq_t)i - cache[sub];
    }
    computed_upto = n;
}

seq_t a(idx_t n)
{
    ensure_computed(n);
    return cache[n];
}
