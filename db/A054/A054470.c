/*
 * A054470  Partial sums of A054469.
 * https://oeis.org/A054470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + (2*n+5)*C(n+4,4)/5 with a(0)=1, a(1)=8,
 *            a(-n)=0
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 33, u64 n <= 79, u128 n <= 172,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Expand the cache up to index n (inclusive) and compute missing values. */
static void compute_up_to(idx_t n)
{
    size_t needed = (size_t)n + 1;

    /* Grow the array if necessary. */
    if (needed > cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap < needed)
            new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        for (size_t i = cache_cap; i < new_cap; ++i)
            cache[i] = (seq_t)0;
        cache_cap = new_cap;
    }

    /* Ensure the base values a(0)=1 and a(1)=8 are present. */
    if (cache_cap < 2)
    {
        size_t new_cap = 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
    }
    else if (cache[0] == (seq_t)0)
    {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
    }

    if (needed <= 2)   /* a(0) and a(1) are already available. */
        return;

    /* Compute subsequent terms using the recurrence. */
    for (size_t i = 2; i <= (size_t)n; ++i)
    {
        idx_t ii = (idx_t)i;

        /* Compute binomial C(ii+4,4). */
        seq_t c = (seq_t)1;
        c = c * ((seq_t)ii + (seq_t)4);
        c = c * ((seq_t)ii + (seq_t)3);
        c = c * ((seq_t)ii + (seq_t)2);
        c = c * ((seq_t)ii + (seq_t)1);
        c = c / (seq_t)24;          /* exact division */

        seq_t term = (((seq_t)2 * ii + (seq_t)5) * c) / (seq_t)5;

        cache[i] = cache[i-1] + cache[i-2] + term;
    }
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[(size_t)n];
}
