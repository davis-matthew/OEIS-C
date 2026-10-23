/*
 * A181328  Number of columns with an even sum in all 2-compositions of n. A
 *          2-composition of n is a nonnegative matrix with two rows, such
 *          that each column has at least one nonzero entry and whose entries
 *          sum up to n.
 * https://oeis.org/A181328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) -5*a(n-2) -16*a(n-3) +8*a(n-4) +8*a(n-5)
 *            -4*a(n-6) with initial values a(0)=0, a(1)=0, a(2)=3, a(3)=12,
 *            a(4)=59, a(5)=248, a(6)=1024. The recurrence follows from the
 *            generating function
 *            G(z)=z^2*(1-z)^2*(3-z^2)/[(1+z)(1-4z+2z^2)]^2 after expanding
 *            the denominator as (1+z)^2*(1-4z+...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t INIT[7] = {0UL, 0UL, 3UL, 12UL, 59UL, 248UL, 1024UL};
static seq_t *cache = NULL;      /* cached values of the sequence   */
static idx_t  cache_len = 0;     /* number of elements currently cached */

/* Extend the cache to contain at least 'needed' elements. */
static void extend_cache(idx_t needed)
{
    if (needed <= cache_len)
        return;

    /* Allocate or grow the buffer. */
    seq_t *new_cache = (seq_t*)realloc(cache, (size_t)needed * sizeof(seq_t));
    if (new_cache == NULL)
        abort(); /* out of memory */

    cache = new_cache;

    /* If this is the first allocation, copy the known initial terms. */
    if (cache_len == 0) {
        for (idx_t i = 0; i < 7; ++i)
            cache[i] = INIT[i];
        cache_len = 7;
    }

    /* Compute missing terms using the linear recurrence. */
    for (idx_t i = cache_len; i < needed; ++i) {
        seq_t pos = cache[i-1];
        pos *= 6;                 /* 6*a(i-1) */
        pos += cache[i-4] * 8;    /* +8*a(i-4) */
        pos += cache[i-5] * 8;    /* +8*a(i-5) */

        seq_t neg = cache[i-2];
        neg *= 5;                 /* 5*a(i-2) */
        neg += cache[i-3] * 16;   /* +16*a(i-3) */
        neg += cache[i-6] * 4;    /* +4*a(i-6) */

        cache[i] = pos - neg;     /* result is non‑negative */
    }

    cache_len = needed;
}

seq_t a(idx_t n)
{
    if (n < 7)
        return INIT[n];

    /* Ensure we have computed up to index n. */
    extend_cache(n + 1);

    return cache[n];
}
