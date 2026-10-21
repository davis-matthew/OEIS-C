/*
 * A224738  Number of (n+1) X 2 0..1 matrices with each 2 X 2 permanent equal.
 * https://oeis.org/A224738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=16, a(2)=38, a(3)=89, a(4)=218, a(5)=538
   a(n) = 5*a(n-1) - 6*a(n-2) - 3*a(n-3) + 7*a(n-4) - 2*a(n-5)   for n > 5 */

#include <stddef.h>
#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_size = 0;      /* allocated buffer size (index 0 unused) */
static size_t max_computed = 0;    /* highest index already stored */

/* initial values for n = 1..5, index 0 unused */
static const seq_t base_vals[6] = {0,
    (seq_t)16,  /* a(1) */
    (seq_t)38,  /* a(2) */
    (seq_t)89,  /* a(3) */
    (seq_t)218, /* a(4) */
    (seq_t)538  /* a(5) */
};

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* out‑of‑range guard */

    /* return initial terms directly if within known range */
    if (n <= 5) return base_vals[n];

    /* ensure cache has room for index n */
    if (cache == NULL || cache_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_size = new_size;
    }

    /* initialise cache with the first five values if not already done */
    if (max_computed < 5) {
        for (idx_t i = 1; i <= 5; ++i) cache[i] = base_vals[i];
        max_computed = 5;
    }

    /* compute all missing terms up to n */
    for (idx_t i = (idx_t)(max_computed + 1); i <= n; ++i) {
        seq_t a1 = cache[i - 1]; /* a(n-1) */
        seq_t a2 = cache[i - 2]; /* a(n-2) */
        seq_t a3 = cache[i - 3]; /* a(n-3) */
        seq_t a4 = cache[i - 4]; /* a(n-4) */
        seq_t a5 = cache[i - 5]; /* a(n-5) */

        /* compute positive part and negative part separately to avoid
           possible unsigned underflow in intermediate subtraction */
        seq_t pos = 5 * a1 + 7 * a4;
        seq_t neg = 6 * a2 + 3 * a3 + 2 * a5;

        cache[i] = pos - neg;
    }

    max_computed = (size_t)n;
    return cache[n];
}
