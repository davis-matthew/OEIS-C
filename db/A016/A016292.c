/*
 * A016292  Expansion of 1/((1-2x)*(1-4x)*(1-10x)).
 * https://oeis.org/A016292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A016292
 * Recurrence: a(n) = 16*a(n-1) - 68*a(n-2) + 80*a(n-3)
 * Initial values: a(0)=1, a(1)=16, a(2)=188
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* dynamic array of computed terms */
static size_t cache_size   = 0;   /* current allocated size */
static size_t last_filled  = 0;   /* one past the last index computed */

/* Ensure cache contains values up to and including n */
static void ensure(idx_t n)
{
    if ((size_t)n + 1 <= cache_size && (size_t)n < last_filled)
        return;                     /* already computed */

    /* Grow the cache to hold at least n+1 elements */
    size_t new_size = (size_t)n + 1;
    if (new_size < 4) new_size = 4;  /* reasonable minimum */
    if (new_size > cache_size)
    {
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);           /* out of memory */
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialize the three base values if not present */
    if (last_filled <= 0)
    {
        cache[0] = (seq_t)1;
        last_filled = 1;
    }
    if (last_filled <= 1)
    {
        cache[1] = (seq_t)16;
        last_filled = 2;
    }
    if (last_filled <= 2)
    {
        cache[2] = (seq_t)188;
        last_filled = 3;
    }

    /* Compute missing values up to n */
    for (size_t i = last_filled; i <= (size_t)n; ++i)
    {
        seq_t val = cache[i-1] * (seq_t)16;
        val -= cache[i-2] * (seq_t)68;
        val += cache[i-3] * (seq_t)80;
        cache[i] = val;
    }
    last_filled = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure(n);
    return cache[(size_t)n];
}
