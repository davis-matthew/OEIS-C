/*
 * A068915  a(n) = n if n<2; a(n) = |a(n/2)-a(n/2-1)| if n is even, and a(n) =
 *          a((n-1)/2) + a((n-1)/2+1) if n is odd.
 * https://oeis.org/A068915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0, a(1) = 1; For n >= 2: if n even: a(n) = |a(n/2) –
 *            a(n/2 – 1)| if n odd : a(n) = a((n-1)/2) + a((n-1)/2 + 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   abs_diff_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/abs_diff_seq.h"

/* Cached values of the sequence.
   capacity  = size of allocated array.
   filled    = highest index computed + 1 (i.e. number of stored terms). */
static seq_t *cache = NULL;
static idx_t capacity = 0;
static idx_t filled   = 0;

/* Ensure cache holds values up to at least 'n'. */
static void ensure_cache(idx_t n)
{
    if (capacity == 0)
    {
        /* Initialise with two terms. */
        capacity = 2;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) abort(); /* Out of memory */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        filled = 2;
    }

    /* Grow the cache if needed. */
    while (n >= capacity)
    {
        idx_t new_capacity = capacity * 2;
        if (new_capacity == 0) new_capacity = 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        if (!tmp) abort(); /* Out of memory */
        cache = tmp;
        capacity = new_capacity;
    }

    /* Compute any missing terms up to n. */
    while (filled <= n)
    {
        idx_t i = filled;
        if (i == 0)
        {
            cache[0] = (seq_t)0;
        }
        else if (i == 1)
        {
            cache[1] = (seq_t)1;
        }
        else
        {
            if ((i & 1) == 0) /* even */
            {
                idx_t h = i / 2;
                cache[i] = abs_diff_seq(cache[h], cache[h - 1]);
            }
            else /* odd */
            {
                idx_t h = (i - 1) / 2;
                cache[i] = cache[h] + cache[h + 1];
            }
        }
        filled++;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
